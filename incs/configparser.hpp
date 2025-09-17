/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configparser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:24:48 by mjong             #+#    #+#             */
/*   Updated: 2025/09/17 15:10:22 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../incs/webserv.hpp"
#include "../incs/configstructs.hpp"

class ConfigParser
{
    public:
        static GlobalConfig parse(const std::string &filename);
};

std::vector<std::string> tokenize(const std::string &line);
static std::string trim(const std::string &s);
