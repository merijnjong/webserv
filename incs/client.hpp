/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:33:24 by mjong             #+#    #+#             */
/*   Updated: 2025/09/18 16:29:16 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

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
        Client(int fd);
        void handleRead();
        void handleWrite();
        int fd;
        std::string in;
        std::string out;
        ClientState state;
        size_t written;
};
