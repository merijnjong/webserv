/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:23:47 by mjong             #+#    #+#             */
/*   Updated: 2025/10/01 18:06:28 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/upload.hpp"

std::string Uploader::sanitizeFilename(const std::string& filename) {
    std::string clean;
    for (size_t i = 0; i < filename.size(); i++) {
        char c = filename[i];
        if (c == '/' || c == '\\') continue;
        if (isspace(c)) continue;
        clean.push_back(c);
    }
    if (clean.empty())
        clean = "upload.bin";
    return clean;
}

std::string Uploader::handleUpload(const Request& req, const LocationConfig& loc) {
    if (req.body.size() > loc.client_max_body_size)
        throw std::runtime_error("413 Payload Too Large");

    std::map<std::string,std::string>::const_iterator it = req.headers.find("Content-Type");
    if (it == req.headers.end())
        throw std::runtime_error("400 Bad Request: missing Content-Type");

    const std::string& ctype = it->second;
    if (ctype.find("multipart/form-data") == 0)
        return saveMultipart(req, loc);
    else if (ctype.find("application/x-www-form-urlencoded") == 0)
        return saveUrlEncoded(req, loc);
    else
        throw std::runtime_error("415 Unsupported Media Type: " + ctype);
}

std::string Uploader::saveUrlEncoded(const Request& req, const LocationConfig& loc) {
    std::string path = loc.upload_store + "/form_" + std::to_string(::time(NULL)) + ".txt";
    std::ofstream ofs(path.c_str(), std::ios::binary);
    if (!ofs.is_open())
        throw std::runtime_error("500 Internal Server Error: cannot write file");
    ofs << req.body;
    ofs.close();
    return path;
}

std::string Uploader::saveMultipart(const Request& req, const LocationConfig& loc) {
    std::string path = loc.upload_store + "/upload_" + std::to_string(::time(NULL)) + ".bin";
    std::ofstream ofs(path.c_str(), std::ios::binary);
    if (!ofs.is_open())
        throw std::runtime_error("500 Internal Server Error: cannot write file");
    ofs << req.body;
    ofs.close();
    return path;
}

