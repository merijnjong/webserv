/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configparser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:21:29 by mjong             #+#    #+#             */
/*   Updated: 2025/09/17 13:26:04 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"
#include "../incs/configparser.hpp"

GlobalConfig ConfigParser::parse(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Could not open config file: " + filename);

    GlobalConfig config;
    std::string line;
    while (std::getline(file, line)) {
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);
        std::istringstream iss(line);
        std::string key, value;
        if (iss >> key >> value)
		{
        	if (key == "port")
				config.port = std::stoi(value);
			else if (key == "host")
				config.host = value;
			else if (key == "root")
				config.root = value;
			else if (key == "index")
				config.index = value;
			else if (key == "error_page")
				config.error_page = value;
			else
				throw std::runtime_error("Unknown configuration key: " + key);
        }
    }
    file.close();
    return (config);
}