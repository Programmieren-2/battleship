//
// Created by rne on 12.06.21.
//

template <typename RequestType>
std::string GameClient::sendMessage(RequestType const &request) const
{
    std::string buf = communicate(serialize(request));
    auto header = deserialize<ResponseHeader>(buf);
    if (header.type == ResponseType::INVALID_REQUEST)
        throw ProtocolError(deserialize<InvalidRequest>(buf).error);

    return buf;
}

template <typename RequestType, typename ResponseType>
ResponseType GameClient::exchangeMessage(RequestType const &request) const
{
    return deserialize<ResponseType>(sendMessage(request));
}