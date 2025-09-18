/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:40:13 by mjong             #+#    #+#             */
/*   Updated: 2025/09/18 13:51:47 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/parsing.hpp"

void handleListen(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    std::string listenStr = tokens[i++];
    size_t colon = listenStr.find(':');
    std::string ip = "0.0.0.0";
    int port = 0;
    if (colon != std::string::npos)
	{
        ip = listenStr.substr(0, colon);
        port = std::atoi(listenStr.substr(colon + 1).c_str());
    } else {
        port = std::atoi(listenStr.c_str());
    }
    if (!isValidPort(port))
        throw std::runtime_error("Invalid port: " + std::to_string(port));
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after listen");
    ++i;
    server.listens.push_back(std::make_pair(ip, port));
}

void handleRoot(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    server.root = tokens[i++];
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after root");
    ++i;
}

void handleIndex(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    server.index = tokens[i++];
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after index");
    ++i;
}

void handleErrorPage(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    int code = std::atoi(tokens[i++].c_str());
    std::string path = tokens[i++];
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after error_page");
    ++i;
    server.error_pages[code] = path;
}

void handleClientMaxBodySize(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    size_t size = std::strtoul(tokens[i++].c_str(), NULL, 10);
    if (size == 0)
        throw std::runtime_error("Invalid client_max_body_size");
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after client_max_body_size");
    ++i;
    server.client_max_body_size = size;
}

void handleLocation(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    LocationConfig location;
    location.path = tokens[i++];
    parseLocationBlock(tokens, i, location);
    server.locations.push_back(location);
}
