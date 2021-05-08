//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_SERVER_H
#define BATTLESHIP_SERVER_H

#include <map>
#include <string>

#include <boost/asio.hpp>

namespace net {
    typedef std::map<unsigned long, std::string> Players;

    class Server {
    private:
        std::string host;
        unsigned int port;
        Players players;
        boost::asio::io_service service;

        boost::asio::ip::tcp::acceptor getAcceptor();
        boost::asio::ip::tcp::socket getSocket();
        std::string receive(boost::asio::ip::tcp::socket &socket);
        void send(boost::asio::ip::tcp::socket &socket, const std::string &message);
        std::string process(std::string const &message);
    public:
        Server(std::string const &host, unsigned int port);
        Server(unsigned int port);
        Server(std::string const &host);
        Server();

        void listen();
    };
}

#endif //BATTLESHIP_SERVER_H
