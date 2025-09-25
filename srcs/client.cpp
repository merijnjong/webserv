/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:37:29 by mjong             #+#    #+#             */
/*   Updated: 2025/09/18 16:30:43 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"
#include "../incs/client.hpp"

Client::Client() : fd(-1), in(""), out(""), state(READING_HEADERS), written(0) {}
Client::Client(int fd) : fd(fd), in(""), out(""), state(READING_HEADERS), written(0) {}

void Client::handleRead() {
    char buffer[4096];
    ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
    if (bytes > 0) {
        in.append(buffer, bytes);
        if (state == READING_HEADERS && in.find("\r\n\r\n") != std::string::npos) {
            state = PROCESSING;
        }
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
