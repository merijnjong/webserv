/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:04:12 by mjong             #+#    #+#             */
/*   Updated: 2025/09/17 15:07:29 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/webserv.hpp"

bool isValidPort(int port)
{
    return port >= 1 && port <= 65535;
}

bool isValidMethod(const std::string& method)
{
    return method == "GET" || method == "POST" || method == "DELETE";
}
