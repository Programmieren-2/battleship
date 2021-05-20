//
// Created by rne on 20.05.21.
//

#ifndef BATTLESHIP_IP_H
#define BATTLESHIP_IP_H

#include <string>
#include <vector>

#include "boostwrap.h"
#include "sockwrap.h"

namespace ip {
    boost::asio::ip::address toAddress(struct addrinfo *addressInfo);
    std::vector<boost::asio::ip::address> getAddresses(std::string const &hostname, int family, int socketType);
    std::vector<boost::asio::ip::address> getAddresses(std::string const &hostname);
    boost::asio::ip::address getAddress(std::string const &hostname, int family, int socketType);
    boost::asio::ip::address getAddress(std::string const &hostname);
};


#endif //BATTLESHIP_IP_H
