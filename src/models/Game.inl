//
// Created by rne on 09.06.21.
//

template <typename PlayerType>
models::Game<PlayerType>::Game(unsigned short width, unsigned short height, ShipTypes shipTypes)
    : width(width), height(height), shipTypes(std::move(shipTypes))
{}

template <typename PlayerType>
models::Game<PlayerType>::Game(unsigned short width, unsigned short height)
    : Game(width, height, SHIP_TYPES)
{}

template <typename PlayerType>
models::Game<PlayerType>::Game()
    : Game(WIDTH, HEIGHT)
{}

template <typename PlayerType>
unsigned short models::Game<PlayerType>::getWidth() const
{
    return width;
}

template <typename PlayerType>
unsigned short models::Game<PlayerType>::getHeight() const
{
    return height;
}

template <typename PlayerType>
ShipTypes models::Game<PlayerType>::getShipTypes() const
{
    return shipTypes;
}

template <typename PlayerType>
std::vector<std::reference_wrapper<PlayerType const>> models::Game<PlayerType>::getPlayers() const
{
    std::vector<std::reference_wrapper<PlayerType const>> result;

    for (auto const &candidate : players) {
        if (candidate)
            result.push_back(*candidate);
    }

    return result;
}

template <typename PlayerType>
unsigned short models::Game<PlayerType>::getPlayerCount() const
{
    unsigned short playerCount = 0;

    for (auto const &candidate : players) {
        if (candidate)
            playerCount++;
    }

    return playerCount;
}

template <typename PlayerType>
std::optional<std::reference_wrapper<PlayerType const>> models::Game<PlayerType>::getPlayer(unsigned short index) const
{
    return *players.at(index % players.size());
}

template <typename PlayerType>
bool models::Game<PlayerType>::hasPlayers() const
{
    return std::ranges::any_of(players, [] (auto const &player) { return player.has_value(); });
}

template <typename PlayerType>
bool models::Game<PlayerType>::isFull() const
{
    return std::ranges::all_of(players, [] (auto const &player) { return player.has_value(); });
}

template <typename PlayerType>
bool models::Game<PlayerType>::isEmpty() const
{
    return std::ranges::none_of(players, [] (auto const &player) { return player.has_value(); });
}

template <typename PlayerType>
bool models::Game<PlayerType>::addPlayer(PlayerType const &player) const
{
    for (auto &candidate : players) {
        if (candidate)
            continue;

        candidate = player;
        return true;
    }

    return false;
}

template <typename PlayerType>
void models::Game<PlayerType>::removePlayer(PlayerType const &player) const
{
    for (auto &candidate : players) {
        if (candidate && *candidate == player)
            candidate.reset();
    }
}

template <typename PlayerType>
ShipTypes const models::Game<PlayerType>::SHIP_TYPES = {
    {"Speed boat", static_cast<unsigned short>(2)},
    {"Destroyer", static_cast<unsigned short>(3)},
    {"Submarine", static_cast<unsigned short>(3)},
    {"Battleship", static_cast<unsigned short>(4)},
    {"Aircraft Carrier", static_cast<unsigned short>(5)},
};