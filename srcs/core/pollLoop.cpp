/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollLoop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:34:12 by mjong             #+#    #+#             */
/*   Updated: 2025/09/25 17:04:41 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/serverCore.hpp"
#include "../../incs/client.hpp"
#include "../../incs/request.hpp"
#include "../../incs/router.hpp"

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags >= 0) {
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
}

void pollLoop(ServerManager& serverManager, GlobalConfig& config) {
    std::map<int, Client> clients;
    while (true) {
        std::vector<struct pollfd> pollfds = serverManager.getPollFds();
        for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
            struct pollfd pfd;
            pfd.fd = it->first;
            pfd.events = POLLIN;
            if (it->second.state == WRITING && !it->second.out.empty()) {
                pfd.events |= POLLOUT;
            }
            pollfds.push_back(pfd);
        }
        int ret = poll(&pollfds[0], pollfds.size(), -1);
        if (ret < 0) {
            std::cerr << "poll() failed: " << strerror(errno) << std::endl;
            break;
        }
        for (size_t i = 0; i < pollfds.size(); ++i) {
            int fd = pollfds[i].fd;
            short revents = pollfds[i].revents;
            if (serverManager.isListeningFd(fd) && (revents & POLLIN)) {
                int clientFd = accept(fd, NULL, NULL);
                if (clientFd >= 0) {
                    setNonBlocking(clientFd);
                    size_t maxBodySize = serverManager.getClientMaxBodySizeForFd(fd);
                    clients[clientFd] = Client(clientFd, maxBodySize, fd);
                }
            } else if (clients.count(fd)) {
                Client& client = clients[fd];
                if (revents & POLLIN) {
                    client.handleRead(serverManager, client.listeningFd, config);
                }
                if (revents & POLLOUT) {
                    client.handleWrite();
                }
                if (client.state == CLOSED) {
                    close(fd);
                    clients.erase(fd);
                }
            }
        }
    }
}
