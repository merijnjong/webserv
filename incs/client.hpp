/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:33:24 by mjong             #+#    #+#             */
/*   Updated: 2025/10/01 17:53:07 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "serverCore.hpp"
#include "configStructs.hpp"
#include "request.hpp"
#include "router.hpp"

enum ClientState {
    READING_HEADERS,
    READING_BODY,
    PROCESSING,
    WRITING,
    CLOSED
};

class Client {
    public:
        Client();
        Client(int fd, size_t maxBodySize, int listeningFd);
        void handleRead(const ServerManager& serverManager, int listeningFd, const GlobalConfig& config);
        void handleWrite();
        void handleHeadersAndRouting(const ServerManager& serverManager, int listeningFd, const GlobalConfig& config);
        void handleBodyAndProcessing();
        int fd;
        size_t written;
        size_t contentLength;
        size_t clientMaxBodySize;
        std::string in;
        std::string out;
        std::string body;
        std::string headers;
        std::string response;
        std::string requestLine;
        ClientState state;
        int listeningFd;
        Request req;
        RouteResult route;
};
