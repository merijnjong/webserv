/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:37:29 by mjong             #+#    #+#             */
/*   Updated: 2025/10/01 15:04:46 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"
#include "../incs/client.hpp"
#include "../incs/request.hpp"
#include "../incs/router.hpp"

Client::Client() :
    fd(-1),
    written(0),
    contentLength(0),
    clientMaxBodySize(0),
    in(""),
    out(""),
    body(""),
    headers(""),
    response(""),
    requestLine(""),
    state(READING_HEADERS)
{}

Client::Client(int fd, size_t maxBodySize, int listeningFd) :
	fd(fd),
	written(0),
	contentLength(0),
	clientMaxBodySize(maxBodySize),
    in(""),
	out(""),
	body(""),
	headers(""),
	response(""),
	requestLine(""),
    state(READING_HEADERS),
	listeningFd(listeningFd)
{}

std::string readFile(const std::string& path) {
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Client::handleHeadersAndRouting(const ServerManager& serverManager, int listeningFd, const GlobalConfig& config) {
    size_t headerEnd = in.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        headerEnd = in.find("\n\n");

    if (state == READING_HEADERS && headerEnd != std::string::npos) {
        std::string headerBlock = in.substr(0, headerEnd);
        size_t firstLineEnd = headerBlock.find("\r\n");
        if (firstLineEnd == std::string::npos)
            firstLineEnd = headerBlock.find("\n");
        requestLine = headerBlock.substr(0, firstLineEnd);
        headers = headerBlock.substr(firstLineEnd + 1);
        size_t serverIdx = serverManager.getServerIdxForFd(listeningFd);
        const ServerConfig& server = config.servers[serverIdx];
        req = parseRequest(requestLine, headers);
        route = resolveRoute(server, req.path);
        if (route.fsPath.empty()) {
            std::string errorBody;
            std::string errorPath;
            if (route.location) {
                errorPath = "errors/403.html";
                errorBody = readFile(errorPath);
                if (errorBody.empty())
                    errorBody = "<html><body><h1>403 Forbidden</h1></body></html>";
                response = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: " +
                           std::to_string(errorBody.size()) + "\r\n\r\n" + errorBody;
            } else {
                errorPath = "errors/404.html";
                errorBody = readFile(errorPath);
                if (errorBody.empty())
                    errorBody = "<html><body><h1>404 Not Found</h1></body></html>";
                response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: " +
                           std::to_string(errorBody.size()) + "\r\n\r\n" + errorBody;
            }
            out = response;
            state = WRITING;
            return;
        }
        size_t clPos = headers.find("Content-Length:");
        if (clPos != std::string::npos) {
            size_t valStart = headers.find_first_not_of(" ", clPos + 15);
            size_t valEnd = headers.find("\r\n", valStart);
            if (valEnd == std::string::npos)
                valEnd = headers.find("\n", valStart);
            contentLength = std::atoi(headers.substr(valStart, valEnd - valStart).c_str());
            state = READING_BODY;
        } else {
            state = PROCESSING;
        }
    }
}

void Client::handleBodyAndProcessing() {
    size_t headerEnd = in.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        headerEnd = in.find("\n\n");
    if (state == READING_BODY && headerEnd != std::string::npos) {
        body = in.substr(headerEnd + ((in[headerEnd] == '\r') ? 4 : 2));
        if (body.size() > clientMaxBodySize) {
            response = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 19\r\n\r\nPayload Too Large\n";
            out = response;
            state = WRITING;
            return;
        }
        if (body.size() >= contentLength) {
            state = PROCESSING;
        }
    }
    if (state == PROCESSING) {
        if (isRegularFile(route.fsPath)) {
            response = buildFileResponse(route.fsPath, "close");
        } else if (isDirectory(route.fsPath)) {
            std::string indexPath = route.fsPath;
            if (indexPath.back() != '/') indexPath += "/";
            indexPath += "index.html";
            if (isRegularFile(indexPath)) {
                response = buildFileResponse(indexPath, "close");
            } else if (route.location && route.location->autoindex) {
                std::string autoIndexHtml = generateAutoIndex(route.fsPath, req.path);
                response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
                    std::to_string(autoIndexHtml.size()) + "\r\n\r\n" + autoIndexHtml;
            } else {
                std::string errorBody = readFile("errors/403.html");
                if (errorBody.empty())
                    errorBody = "<html><body><h1>403 Forbidden</h1></body></html>";
                response = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: " +
                    std::to_string(errorBody.size()) + "\r\n\r\n" + errorBody;
            }
        } else {
            std::string errorBody = readFile("errors/404.html");
            if (errorBody.empty())
                errorBody = "<html><body><h1>404 Not Found</h1></body></html>";
            response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: " +
                std::to_string(errorBody.size()) + "\r\n\r\n" + errorBody;
        }
        out = response;
        state = WRITING;
    }
}

void Client::handleRead(const ServerManager& serverManager, int listeningFd, const GlobalConfig& config) {
    char buffer[4096];
    ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
    if (bytes > 0) {
        in.append(buffer, bytes);
        handleHeadersAndRouting(serverManager, listeningFd, config);
        handleBodyAndProcessing();
    } else if (bytes == 0) {
        state = CLOSED;
    } else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Read error on fd " << fd << ": " << strerror(errno) << std::endl;
            state = CLOSED;
        }
    }
}

void Client::handleWrite() {
    if (out.empty()) {
        return;
    }
    ssize_t bytes = send(fd, out.data() + written, out.size() - written, 0);
    if (bytes > 0) {
        written += bytes;
        if (written >= out.size()) {
            out.clear();
            written = 0;
            state = CLOSED;
        }
    } else if (bytes < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Write error on fd " << fd << ": " << strerror(errno) << std::endl;
            state = CLOSED;
        }
    }
}
