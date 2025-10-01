/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 14:58:50 by mjong             #+#    #+#             */
/*   Updated: 2025/10/01 15:38:12 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/webserv.hpp"

std::string getMimeType(const std::string& path) {
    static std::map<std::string, std::string> mime = {
        {".html", "text/html"}, {".htm", "text/html"},
        {".css", "text/css"}, {".js", "application/javascript"},
        {".png", "image/png"}, {".jpg", "image/jpeg"}, {".jpeg", "image/jpeg"},
        {".gif", "image/gif"}, {".txt", "text/plain"}, {".json", "application/json"}
    };
    size_t dot = path.rfind('.');
    if (dot != std::string::npos) {
        std::string ext = path.substr(dot);
        if (mime.count(ext)) return mime[ext];
    }
    return ("application/octet-stream");
}
