/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:00:00 by mjong             #+#    #+#             */
/*   Updated: 2025/10/08 16:14:22 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incs/deleteHandler.hpp"

static bool isPathWithinRoot(const std::string& root, const std::string& target) {
    char resolvedRoot[PATH_MAX];
    char resolvedTarget[PATH_MAX];

    if (!realpath(root.c_str(), resolvedRoot))
        return false;
    if (!realpath(target.c_str(), resolvedTarget))
        return false;

    std::string rootPath(resolvedRoot);
    std::string targetPath(resolvedTarget);

    return targetPath.find(rootPath) == 0;
}

std::string handleDelete(const Request& req, const LocationConfig& loc) {
    if (std::find(loc.methods.begin(), loc.methods.end(), "DELETE") == loc.methods.end()) {
        return makeResponse(405, "Method Not Allowed", "DELETE not allowed for this location\n");
    }
    std::string fullPath = loc.root + req.path;
    if (!isPathWithinRoot(loc.root, fullPath)) {
        return makeResponse(403, "Forbidden", "Attempted path escape detected\n");
    }
    struct stat st;
    if (stat(fullPath.c_str(), &st) != 0) {
        return makeResponse(404, "Not Found", "File not found\n");
    }
    if (S_ISDIR(st.st_mode)) {
        return makeResponse(403, "Forbidden", "Cannot delete a directory\n");
    }
    if (unlink(fullPath.c_str()) != 0) {
        return makeResponse(500, "Internal Server Error", "Failed to delete file\n");
    }
    return makeResponse(200, "OK", "File deleted successfully\n");
}