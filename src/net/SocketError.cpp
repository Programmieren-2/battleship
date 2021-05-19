//
// Created by rne on 12.05.21.
//

#include <SDKDDKVer.h>
#include <boost/asio.hpp>
using boost::system::error_code;

#include "SocketError.h"

namespace net {
    SocketError::SocketError(error_code const error)
            : error(error)
    {}

    error_code SocketError::getError() const noexcept
    {
        return error;
    }

    const char *SocketError::what() const noexcept
    {
        return "socket error";
    }
}