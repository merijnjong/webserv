/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 15:00:36 by mjong             #+#    #+#             */
/*   Updated: 2025/10/01 17:45:17 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/router.hpp"
#include "../../incs/upload.hpp"

bool fileExists(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

bool isRegularFile(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

bool isDirectory(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

std::string generateAutoIndex(const std::string& dirPath, const std::string& urlPath) {
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) return "<html><body>Cannot open directory</body></html>";
    std::stringstream ss;
    ss << "<html><body><h1>Index of " << urlPath << "</h1><ul>";
    struct dirent* entry;
    while ((entry = readdir(dir))) {
        std::string name = entry->d_name;
        if (name == ".") continue;
        ss << "<li><a href=\"" << urlPath;
        if (urlPath.back() != '/') ss << "/";
        ss << name << "\">" << name << "</a></li>";
    }
    closedir(dir);
    ss << "</ul></body></html>";
    return ss.str();
}

std::string makeResponse(int status, const std::string& statusMsg,
                         const std::string& body,
                         const std::string& contentType = "text/plain",
                         const std::string& connection = "close")
{
    std::ostringstream resp;
    resp << "HTTP/1.1 " << status << " " << statusMsg << "\r\n"
         << "Content-Length: " << body.size() << "\r\n"
         << "Content-Type: " << contentType << "\r\n"
         << "Connection: " << connection << "\r\n\r\n"
         << body;
    return resp.str();
}

std::string buildFileResponse(const std::string& path,
                              const std::string& connection = "close")
{
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
        return makeResponse(404, "Not Found", "File not found\n");

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string body = ss.str();

    std::string mime = getMimeType(path);
    return makeResponse(200, "OK", body, mime, connection);
}

std::string handleRequest(const Request& req, const LocationConfig& locationConfig) {
    if (req.method == "GET") {
        std::string fullPath = locationConfig.root + req.path;
        if (isDirectory(fullPath)) {
            if (locationConfig.autoindex)
                return makeResponse(200, "OK",
                                    generateAutoIndex(fullPath, req.path),
                                    "text/html");
            else if (!locationConfig.index.empty())
                return buildFileResponse(fullPath + "/" + locationConfig.index);
            else
                return makeResponse(403, "Forbidden", "Directory listing denied\n");
        }
        else if (isRegularFile(fullPath))
            return buildFileResponse(fullPath);
        else
            return makeResponse(404, "Not Found", "File not found\n");
    }
    else if (req.method == "POST") {
        try {
            std::string saved = Uploader::handleUpload(req, locationConfig);
            std::string body = "File uploaded to " + saved + "\n";
            return makeResponse(201, "Created", body);
        } catch (const std::exception& e) {
            std::string body = std::string("Upload failed: ") + e.what() + "\n";
            return makeResponse(400, "Bad Request", body);
        }
    }
    else if (req.method == "DELETE") {
        std::string fullPath = locationConfig.root + req.path;
        if (!isRegularFile(fullPath))
            return makeResponse(404, "Not Found", "File not found\n");

        if (remove(fullPath.c_str()) == 0)
            return makeResponse(200, "OK", "File deleted\n");
        else
            return makeResponse(500, "Internal Server Error", "Failed to delete file\n");
    }
    else {
        return makeResponse(405, "Method Not Allowed", "Unsupported method\n");
    }
}
