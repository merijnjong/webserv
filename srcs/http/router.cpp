/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:54:22 by mjong             #+#    #+#             */
/*   Updated: 2025/09/25 17:30:31 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/router.hpp"

static std::string normalizePath(const std::string& path) {
    std::vector<std::string> segments;
    std::istringstream iss(path);
    std::string seg;
    while (std::getline(iss, seg, '/')) {
        if (seg.empty() || seg == ".") continue;
        if (seg == "..") {
            if (!segments.empty()) segments.pop_back();
        } else {
            segments.push_back(seg);
        }
    }
    std::string out = "/";
    for (size_t i = 0; i < segments.size(); ++i) {
        out += segments[i];
        if (i + 1 < segments.size()) out += "/";
    }
    return (out);
}

std::string safeJoin(const std::string& root, const std::string& subpath) {
    std::string joined = root;
    if (joined.back() != '/') joined += "/";
    joined += subpath;
    std::string norm = normalizePath(joined);
    std::string normRoot = normalizePath(root);
    if (norm.compare(0, normRoot.size(), normRoot) != 0)
        return (""); 
    return norm;
}

RouteResult resolveRoute(const ServerConfig& server, const std::string& reqPath) {
    const LocationConfig* bestLoc = NULL;
    size_t bestLen = 0;
    for (size_t i = 0; i < server.locations.size(); ++i) {
        const LocationConfig& loc = server.locations[i];
        if (reqPath.compare(0, loc.path.size(), loc.path) == 0 && loc.path.size() > bestLen) {
            bestLoc = &loc;
            bestLen = loc.path.size();
        }
    }
    std::string fsPath;
    if (bestLoc) {
        std::string remainder = reqPath.substr(bestLoc->path.size());
        std::string root = bestLoc->root.empty() ? server.root : bestLoc->root;
        fsPath = safeJoin(root, remainder);
        if (fsPath.empty()) {
            return (RouteResult{"", bestLoc});
        }
    } else {
        fsPath = safeJoin(server.root, reqPath);
        if (fsPath.empty()) {
            return (RouteResult{"", NULL});
        }
    }
    return (RouteResult{fsPath, bestLoc});
}
