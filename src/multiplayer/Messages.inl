//
// Created by rne on 09.06.21.
//

template <typename Message>
Message deserialize(std::string const &buf, bool partialProcessing)
{
    Message msg;
    size_t bufSize = buf.length();
    size_t msgSize = sizeof msg;

    if (msgSize > bufSize)
        throw std::underflow_error("Message size exceeds buffer size.");

    if (msgSize < bufSize && !partialProcessing)
        throw std::overflow_error("Buffer size exceeds message size.");

    std::memcpy(&msg, &buf[0], (msgSize < bufSize) ? msgSize : bufSize);
    return msg;
}

template <typename Message>
std::string serialize(Message const &msg)
{
    auto ptr = reinterpret_cast<const char*>(&msg);
    return std::string(ptr, ptr + sizeof msg);
}