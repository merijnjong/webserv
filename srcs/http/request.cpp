/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:47:55 by mjong             #+#    #+#             */
/*   Updated: 2025/09/25 17:10:18 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/request.hpp"

std::string normalizeHeader(const std::string& name) {
    std::string out = name;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return (out);
}

Request parseRequest(const std::string& requestLine, const std::string& headersBlock) {
    Request req;
    std::istringstream rl(requestLine);
    rl >> req.method >> req.path >> req.httpVersion;
    size_t qpos = req.path.find('?');
    if (qpos != std::string::npos) {
        req.query = req.path.substr(qpos + 1);
        req.path = req.path.substr(0, qpos);
    }
    std::istringstream hl(headersBlock);
    std::string line;
    size_t totalHeaderSize = 0;
    while (std::getline(hl, line)) {
        totalHeaderSize += line.size();
        if (totalHeaderSize > 8192) break;
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string name = normalizeHeader(line.substr(0, colon));
            std::string value = line.substr(colon + 1);
            size_t start = value.find_first_not_of(" \t");
            if (start != std::string::npos)
                value = value.substr(start);
            req.headers[name] = value;
        }
    }
    return (req);
}
