/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configstructs.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:43:53 by mjong             #+#    #+#             */
/*   Updated: 2025/09/17 15:00:55 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include <map>

struct LocationConfig {
    std::vector<std::string> methods;
    std::string path;
    std::string redirect;
    std::string root;
    std::string index;
    std::string upload_store;
    bool autoindex;
};

struct ServerConfig {
    std::vector<std::pair<std::string, int>> listens;
    std::vector<LocationConfig> locations;
    std::map<int, std::string> error_pages;
    std::string root;
    std::string index;
    size_t client_max_body_size;
};

struct GlobalConfig {
    std::vector<ServerConfig> servers;
};

