/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:24:48 by mjong             #+#    #+#             */
/*   Updated: 2025/09/18 13:50:01 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "configStructs.hpp"

class ConfigParser
{
    public:
        static GlobalConfig parse(const std::string &filename);
};

// ../srcs/parsing/config.cpp
void parseLocationBlock(const std::vector<std::string>& tokens, size_t& i, LocationConfig& location);

// ../srcs/parsing/handle.cpp
void handleListen(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server);
void handleRoot(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server);
void handleIndex(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server);
void handleErrorPage(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server);
void handleClientMaxBodySize(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server);
void handleLocation(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server);

//../srcs/parsing/tokanization.cpp
std::vector<std::string> tokenize(const std::string &line);
std::string trim(const std::string &s);
