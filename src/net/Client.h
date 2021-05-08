//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_CLIENT_H
#define BATTLESHIP_CLIENT_H

#include <string>

#include <boost/asio.hpp>

namespace net {
    class Client {
    private:
        boost::system::error_code send(boost::asio::ip::tcp::socket &socket, string const &message);
        std::string receive(boost::asio::ip::tcp::socket &socket);
    public:
        std::string communicate(std::string const &host, unsigned int port, std::string const &message);
        std::string communicate(unsigned int port, std::string const &message);
        std::string communicate(std::string const &host, std::string const &message);
        std::string communicate(std::string const &message);
    };
}

#endif //BATTLESHIP_CLIENT_H
