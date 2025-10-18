include_directories(../include)

add_library(GameLibrary STATIC
    Field.cpp
    Ship.cpp
    ShipFunc.cpp
    ShipShape.cpp
)

add_executable(Battleship
    main.cpp
)

target_link_libraries(Battleship PRIVATE
    GameLibrary
)