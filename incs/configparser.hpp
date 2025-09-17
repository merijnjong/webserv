/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configparser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:24:48 by mjong             #+#    #+#             */
/*   Updated: 2025/09/17 14:06:15 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../incs/webserv.hpp"

struct GlobalConfig
{
	std::string host;
    std::string root;
    std::string index;
    std::string error_page;
    int port;
};

class ConfigParser
{
	public:
		static GlobalConfig parse(const std::string &filename);
};
