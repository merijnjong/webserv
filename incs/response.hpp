/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:09:29 by mjong             #+#    #+#             */
/*   Updated: 2025/10/08 16:09:58 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

std::string makeResponse(int status, const std::string& statusMsg,
                         const std::string& body,
                         const std::string& contentType = "text/plain",
                         const std::string& connection = "close");
