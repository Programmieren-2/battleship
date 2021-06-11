add_library(models
        BasicShip.cpp BasicShip.h
        bootstrap.cpp bootstrap.h bootstrap.inl
        Coordinate.cpp Coordinate.h
        exceptions.cpp exceptions.h
        Game.h Game.inl
        HitPoint.cpp HitPoint.h
        HitResult.h
        Matrix.h
        Orientation.h
        Player.cpp Player.h
        Sea.cpp Sea.h
        Ship.cpp Ship.h
        ShipTypes.h
)
target_include_directories(models PUBLIC ../util)
target_link_libraries(models util)