/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:21:29 by mjong             #+#    #+#             */
/*   Updated: 2025/09/18 13:51:30 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/parsing.hpp"

void parseLocationBlock(const std::vector<std::string>& tokens, size_t& i, LocationConfig& location) {
    if (tokens[i] != "{") throw std::runtime_error("Expected '{' after location");
	++i;
    while (i < tokens.size() && tokens[i] != "}") {
        std::string key = tokens[i++];
        if (key == "methods") {
            while (tokens[i] != ";") {
                if (!isValidMethod(tokens[i]))
                    throw std::runtime_error("Invalid method: " + tokens[i]);
                location.methods.push_back(tokens[i++]);
            }
            ++i;
        } else if (key == "redirect") {
            location.redirect = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after redirect");
            ++i;
        } else if (key == "root") {
            location.root = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after root");
            ++i;
        } else if (key == "autoindex") {
            std::string val = tokens[i++];
            if (val == "on") location.autoindex = true;
            else if (val == "off") location.autoindex = false;
            else throw std::runtime_error("Invalid autoindex value: " + val);
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after autoindex");
            ++i;
        } else if (key == "index") {
            location.index = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after index");
            ++i;
        } else if (key == "upload_store") {
            location.upload_store = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after upload_store");
            ++i;
        } else {
            throw std::runtime_error("Unknown directive in location: " + key);
        }
    }
    if (tokens[i] != "}") throw std::runtime_error("Missing '}' at end of location block");
    ++i;
}

static void parseServerBlock(const std::vector<std::string>& tokens, size_t& i, GlobalConfig& config) {
    if (tokens[i] != "{")
        throw std::runtime_error("Expected '{' after server");
    ++i;
    ServerConfig server;
    server.index = "index.html";
    server.client_max_body_size = 1024 * 1024; // default 1MB
    while (i < tokens.size() && tokens[i] != "}") {
        std::string key = tokens[i++];
        if (key == "listen")
            handleListen(tokens, i, server);
        else if (key == "root")
            handleRoot(tokens, i, server);
        else if (key == "index")
            handleIndex(tokens, i, server);
        else if (key == "error_page")
            handleErrorPage(tokens, i, server);
        else if (key == "client_max_body_size")
            handleClientMaxBodySize(tokens, i, server);
        else if (key == "location")
            handleLocation(tokens, i, server);
        else
            throw std::runtime_error("Unknown directive inside server: " + key);
    }
    if (tokens[i] != "}")
        throw std::runtime_error("Missing '}' at end of server block");
    ++i;
    config.servers.push_back(server);
}

GlobalConfig ConfigParser::parse(const std::string &filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Could not open config file: " + filename);
    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(file, line)) {
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);
        line = trim(line);
        if (line.empty()) continue;
        std::vector<std::string> lineTokens = tokenize(line);
        tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
    }
    file.close();
    GlobalConfig config;
    size_t i = 0;
    while (i < tokens.size()) {
        if (tokens[i] == "server") {
            ++i;
            parseServerBlock(tokens, i, config);
        } else {
            throw std::runtime_error("Unknown top-level directive: " + tokens[i]);
        }
    }
    for (size_t s = 0; s < config.servers.size(); ++s) {
        if (config.servers[s].index.empty()) {
            config.servers[s].index = "index.html";
        } if (config.servers[s].client_max_body_size == 0) {
            config.servers[s].client_max_body_size = 1024 * 1024;
        } if (config.servers[s].error_pages.find(404) == config.servers[s].error_pages.end()) {
            config.servers[s].error_pages[404] = "errors/404.html";
        } if (config.servers[s].error_pages.find(500) == config.servers[s].error_pages.end()) {
            config.servers[s].error_pages[500] = "errors/500.html";
        }
    }
    return config;
}
