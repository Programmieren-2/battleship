# Battleship
A simple console based battleship game.

# Usage
The game can be used in a local hot-seat mode and in a multiplayer mode (WIP).

## Hot-Seat
Run `battleship` to interactively set up a local hot-seat game.

## Multiplayer
The multiplayer works with a classic client / server architecture.
Note, that the network multiplayer mode is work in progress and not guaranteed to work.

### Server
To start a server, run

    bssrv <address> <port>

### Client
To connect to a server with the client, run

    bsclt <address> <port>

**CAUTION:** The multiplayer currently works with an unencrypted TCP connection.  
Do not send critical information via the multiplayer connections.

# Credits
This project was inspired by exercise #3 from the lecture "Programmieren 2" at TU Braunschweig during the summer semester of 2021.    
Credits to Dr. Steve Grogorick on whose idea and work this rewrite is based.