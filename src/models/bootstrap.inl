//
// Created by rne on 12.06.21.
//

template <typename PlayerType>
PlayerType models::readPlayer(unsigned short width, unsigned short height)
{
    static unsigned short playerNum = 0;
    playerNum++;
    string name = util::readWithPrompt("Enter name of player #" + std::to_string(playerNum) + ": ");
    Sea sea = readSea(width, height);
    cout << sea.toString(true);
    return PlayerType(name, sea);
}

template <typename PlayerType>
Game<PlayerType> models::readGame()
{
    Game<PlayerType> game;
    unsigned short width = game.getWidth();
    unsigned short height = game.getHeight();

    PlayerType player = readPlayer<PlayerType>(width, height);
    game.addPlayer(player);
    player = readPlayer<PlayerType>(width, height);
    game.addPlayer(player);
    return game;
}