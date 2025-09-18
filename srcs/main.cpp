/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjong <mjong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:53:18 by mjong             #+#    #+#             */
/*   Updated: 2025/09/18 13:04:22 by mjong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/webserv.hpp"
#include "../incs/parsing.hpp"

int main(int argc, char **argv) {
    std::string configFile = "config/default.conf";
    if (argc == 2) {
        configFile = argv[1];
    }
    try {
        GlobalConfig config = ConfigParser::parse(configFile);
        std::cout << "Config parsed successfully!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error parsing config: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
