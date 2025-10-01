/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:53:00 by mjong             #+#    #+#             */
/*   Updated: 2025/10/01 15:39:16 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "configStructs.hpp"
#include "request.hpp"

struct RouteResult {
    std::string fsPath;
    const LocationConfig* location;
};

RouteResult resolveRoute(const ServerConfig& server, const std::string& reqPath);
std::string buildFileResponse(const std::string& path, const std::string& connection);
std::string generateAutoIndex(const std::string& dirPath, const std::string& urlPath);
std::string safeJoin(const std::string& root, const std::string& subpath);
std::string getMimeType(const std::string& path);
bool isRegularFile(const std::string& path);
bool isDirectory(const std::string& path);
