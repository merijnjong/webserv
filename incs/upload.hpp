/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:22:22 by mjong             #+#    #+#             */
/*   Updated: 2025/10/01 17:51:21 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "configStructs.hpp"
#include "request.hpp"

class Uploader {
public:
    static std::string handleUpload(const Request& req, const LocationConfig& locationConfig);

private:
    static std::string saveUrlEncoded(const Request& req, const LocationConfig& loc);
    static std::string sanitizeFilename(const std::string& filename);
    static std::string saveMultipart(const Request& req, const LocationConfig& loc);
};