//
// Created by rne on 12.05.21.
//

#ifndef BATTLESHIP_NOSUCHPLAYER_H
#define BATTLESHIP_NOSUCHPLAYER_H

#include <exception>

namespace proto {
    class NoSuchPlayer : std::exception{
    private:
        unsigned long playerId;
    public:
        explicit NoSuchPlayer(unsigned long playerId);
        [[nodiscard]] unsigned long getPlayerId() const noexcept;
        [[nodiscard]] const char *what() const noexcept override;
    };
}

#endif //BATTLESHIP_NOSUCHPLAYER_H
