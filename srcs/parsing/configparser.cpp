/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configparser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:21:29 by mjong             #+#    #+#             */
/*   Updated: 2025/09/17 15:13:07 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/webserv.hpp"
#include "../../incs/configparser.hpp"

static void parseLocationBlock(const std::vector<std::string>& tokens, size_t& i, LocationConfig& location)
{
    if (tokens[i] != "{") throw std::runtime_error("Expected '{' after location");
    ++i;
    while (i < tokens.size() && tokens[i] != "}")
	{
        std::string key = tokens[i++];
        if (key == "methods") {
            while (tokens[i] != ";") {
                if (!isValidMethod(tokens[i]))
                    throw std::runtime_error("Invalid method: " + tokens[i]);
                location.methods.push_back(tokens[i++]);
            }
            ++i;
        } 
		else if (key == "redirect")
		{
            location.redirect = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after redirect");
            ++i;
        } 
		else if (key == "root")
		{
            location.root = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after root");
            ++i;
        }
		else if (key == "autoindex")
		{
            std::string val = tokens[i++];
            if (val == "on") location.autoindex = true;
            else if (val == "off") location.autoindex = false;
            else throw std::runtime_error("Invalid autoindex value: " + val);
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after autoindex");
            ++i;
        } 
		else if (key == "index")
		{
            location.index = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after index");
            ++i;
        } 
		else if (key == "upload_store")
		{
            location.upload_store = tokens[i++];
            if (tokens[i] != ";") throw std::runtime_error("Missing ';' after upload_store");
            ++i;
        } 
		else
		{
            throw std::runtime_error("Unknown directive in location: " + key);
        }
    }
    if (tokens[i] != "}") throw std::runtime_error("Missing '}' at end of location block");
    ++i;
}

static void handleListen(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
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

static void handleRoot(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    server.root = tokens[i++];
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after root");
    ++i;
}

static void handleIndex(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    server.index = tokens[i++];
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after index");
    ++i;
}

static void handleErrorPage(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    int code = std::atoi(tokens[i++].c_str());
    std::string path = tokens[i++];
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after error_page");
    ++i;
    server.error_pages[code] = path;
}

static void handleClientMaxBodySize(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    size_t size = std::strtoul(tokens[i++].c_str(), NULL, 10);
    if (size == 0)
        throw std::runtime_error("Invalid client_max_body_size");
    if (tokens[i] != ";")
        throw std::runtime_error("Missing ';' after client_max_body_size");
    ++i;
    server.client_max_body_size = size;
}

static void handleLocation(const std::vector<std::string>& tokens, size_t& i, ServerConfig& server)
{
    LocationConfig location;
    location.path = tokens[i++];
    parseLocationBlock(tokens, i, location);
    server.locations.push_back(location);
}

static void parseServerBlock(const std::vector<std::string>& tokens, size_t& i, GlobalConfig& config)
{
    if (tokens[i] != "{")
        throw std::runtime_error("Expected '{' after server");
    ++i;
    ServerConfig server;
    server.index = "index.html";
    server.client_max_body_size = 1024 * 1024; // default 1MB
    while (i < tokens.size() && tokens[i] != "}")
    {
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

GlobalConfig ConfigParser::parse(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Could not open config file: " + filename);
    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(file, line))
    {
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
    while (i < tokens.size())
    {
        if (tokens[i] == "server")
        {
            ++i;
            parseServerBlock(tokens, i, config);
        }
        else
        {
            throw std::runtime_error("Unknown top-level directive: " + tokens[i]);
        }
    }
    for (size_t s = 0; s < config.servers.size(); ++s)
	{
        if (config.servers[s].index.empty())
            config.servers[s].index = "index.html";
    }
    return config;
}
