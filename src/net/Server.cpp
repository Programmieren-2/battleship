//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::buffer;
using boost::asio::buffer_cast;
using boost::asio::io_service;
using boost::asio::ip::address;
using boost::asio::ip::tcp;
using boost::asio::read_until;
using boost::asio::streambuf;
using boost::asio::write;

#include "Defaults.h"
#include "Server.h"

namespace net {
    Server::Server(string const &host, unsigned int port)
    : host(host), port(port), players(Players()), service(io_service())
    {}

    Server::Server(unsigned int port)
    : Server(Defaults::HOST, port)
    {}

    Server::Server(string const &host)
    : Server(host, Defaults::PORT)
    {}

    Server::Server()
    : Server(Defaults::HOST, Defaults::PORT)
    {}

    tcp::acceptor Server::getAcceptor()
    {
        return tcp::acceptor(service, tcp::endpoint(address::from_string(host), port));
    }

    tcp::socket Server::getSocket()
    {
        tcp::socket socket(service);
        tcp::acceptor acceptor = getAcceptor();
        acceptor.accept(socket);
        return socket;
    }

    string Server::process(string const &message)
    {
        string reply = "Hi there! You sent '" + message + "'.";
        return reply;
    }

    string Server::receive(tcp::socket &socket)
    {
        streambuf buf;
        read_until(socket, buf, "\n" );
        string result = buffer_cast<const char*>(buf.data());
        return result.substr(0, result.length() - 1);
    }

    void Server::send(tcp::socket &socket, string const &message)
    {
        write(socket, buffer(message + "\n"));
    }

    void Server::listen()
    {
        string message, reply;

        while (true) {
            tcp::socket socket = getSocket();
            message = receive(socket);
            reply = process(message);
            send(socket, reply);

            if (message == "terminate")
                break;
        }
    }
}
