//
// Created by rne on 20.05.21.
//

#include <stdexcept>
using std::domain_error;
using std::invalid_argument;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "boostwrap.h"
using boost::asio::ip::address;
using boost::asio::ip::address_v4;
using boost::asio::ip::address_v6;

#include "sockwrap.h"

#include "ip.h"

namespace ip {
    static address_v4 toIPv4(struct sockaddr_in *socketAddressIPv4)
    {
        return address_v4(htonl(socketAddressIPv4->sin_addr.s_addr));
    }

    static address_v4 toIPv4(struct sockaddr *socketAddress)
    {
        return toIPv4(reinterpret_cast<struct sockaddr_in*>(socketAddress));
    }

    static address_v6 toIPv6(struct sockaddr_in6* socketAddressIPv6)
    {
        auto &bytes = reinterpret_cast<std::array<unsigned char, 16>&>(socketAddressIPv6->sin6_addr.s6_addr);
        return address_v6(bytes, socketAddressIPv6->sin6_scope_id);
    }

    static address_v6 toIPv6(struct sockaddr *socketAddress)
    {
        return toIPv6(reinterpret_cast<struct sockaddr_in6*>(socketAddress));
    }

    address toAddress(struct addrinfo *addressInfo)
    {
        if (addressInfo->ai_family == AF_INET)
            return toIPv4(addressInfo->ai_addr);

        if (addressInfo->ai_family == AF_INET6)
            return toIPv6(addressInfo->ai_addr);

        throw invalid_argument("Not an IPv4 or IPv6 address.");
    }

    static vector<address> getAddresses(struct addrinfo *pai)
    {
        vector<address> addresses;

        for(struct addrinfo *info = pai; info != nullptr; info = info->ai_next) {
            try {
                addresses.emplace_back(toAddress(info));
            } catch (invalid_argument const &) {
                continue;
            }
        }

        return addresses;
    }

    vector<address> getAddresses(string const &hostname, int family, int socketType)
    {
        struct addrinfo req;
        req.ai_family = family;
        req.ai_socktype = socketType;
        struct addrinfo *pai;
        int error = getaddrinfo(hostname.c_str(), nullptr, &req, &pai);
        if (error)
            throw domain_error("Could not resolve host name.");

        return getAddresses(pai);
    }

    vector<address> getAddresses(string const &hostname)
    {
        return getAddresses(hostname, AF_UNSPEC, SOCK_STREAM);
    }

    address getAddress(string const &hostname, int family, int socketType)
    {
        return getAddresses(hostname, family, socketType).at(0);
    }

    address getAddress(string const &hostname)
    {
        return getAddress(hostname, AF_UNSPEC, SOCK_STREAM);
    }
}
