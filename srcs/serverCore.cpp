/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverCore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:56:57 by mjong             #+#    #+#             */
/*   Updated: 2025/09/18 15:32:20 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/serverCore.hpp"
#include "../incs/webserv.hpp"

ServerManager::ServerManager(const GlobalConfig& config) : _config(config) {}

bool ServerManager::setupListeningSockets() {
    for (size_t s = 0; s < _config.servers.size(); ++s) {
        const ServerConfig& server = _config.servers[s];
        for (size_t l = 0; l < server.listens.size(); ++l) {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                std::cerr << "socket() failed: " << strerror(errno) << std::endl;
                return false;
            }
            int opt = 1;
            if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                std::cerr << "setsockopt() failed: " << strerror(errno) << std::endl;
                close(sock);
                return false;
            }
            sockaddr_in addr;
            std::memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(server.listens[l].second);
            if (server.listens[l].first == "0.0.0.0") {
                addr.sin_addr.s_addr = htonl(INADDR_ANY);
            } else {
                if (inet_pton(AF_INET, server.listens[l].first.c_str(), &addr.sin_addr) <= 0) {
                    std::cerr << "inet_pton() failed for " << server.listens[l].first << std::endl;
                    close(sock);
                    return (false);
                }
            }
            if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
                std::cerr << "bind() failed: " << strerror(errno) << std::endl;
                close(sock);
                return (false);
            }
            if (listen(sock, SOMAXCONN) < 0) {
                std::cerr << "listen() failed: " << strerror(errno) << std::endl;
                close(sock);
                return (false);
            }
            int flags = fcntl(sock, F_GETFL, 0);
            if (flags < 0 || fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
                std::cerr << "fcntl() failed: " << strerror(errno) << std::endl;
                close(sock);
                return (false);
            }
            _fdToServer[sock] = s;
            struct pollfd pfd;
            pfd.fd = sock;
            pfd.events = POLLIN;
            _pollfds.push_back(pfd);
        }
    }
    return (true);
}

std::vector<struct pollfd> ServerManager::getPollFds() const {
    return _pollfds;
}

bool ServerManager::isListeningFd(int fd) const {
    return _fdToServer.count(fd) > 0;
}