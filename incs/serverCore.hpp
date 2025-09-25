/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverCore.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:54:21 by mjong             #+#    #+#             */
/*   Updated: 2025/09/25 17:05:23 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "parsing.hpp"

class ServerManager {
    public:
        ServerManager(const GlobalConfig& config);
        bool setupListeningSockets();
        std::vector<struct pollfd> getPollFds() const;
        bool isListeningFd(int fd) const;
        size_t getServerIdxForFd(int fd) const;
        size_t getClientMaxBodySizeForFd(int fd) const;
    private:
        const GlobalConfig& _config;
        std::map<int, size_t> _fdToServer;
        std::vector<struct pollfd> _pollfds;
};

void pollLoop(ServerManager& serverManager, GlobalConfig& config);
