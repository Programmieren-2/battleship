add_library(models
        BasicShip.cpp BasicShip.h
        bootstrap.cpp bootstrap.h
        Coordinate.cpp Coordinate.h
        Game.h
        HitPoint.cpp HitPoint.h
        Defaults.h
        Player.cpp Player.h
        Sea.cpp Sea.h
        Ship.cpp Ship.h Matrix.h exceptions.cpp exceptions.h Orientation.h HitResult.h ShipTypes.h)
target_include_directories(models PUBLIC ../util)
target_link_libraries(models util)