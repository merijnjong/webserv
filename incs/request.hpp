/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:46:52 by mjong             #+#    #+#             */
/*   Updated: 2025/09/25 17:30:02 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

struct Request {
    std::string method;
    std::string path;
    std::string query;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
};

Request parseRequest(const std::string& requestLine, const std::string& headersBlock);
std::string normalizeHeader(const std::string& name);
