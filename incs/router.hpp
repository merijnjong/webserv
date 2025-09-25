/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:53:00 by mjong             #+#    #+#             */
/*   Updated: 2025/09/25 16:53:03 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configStructs.hpp"
#include "request.hpp"

struct RouteResult {
    std::string fsPath;
    const LocationConfig* location;
};

RouteResult resolveRoute(const ServerConfig& server, const std::string& reqPath);
std::string safeJoin(const std::string& root, const std::string& subpath);
