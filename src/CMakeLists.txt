add_subdirectory(Box2D)
add_subdirectory(BomberBlob)

add_executable(game
        main.cpp
		GameMenu.cpp GameMenu.hpp)

target_link_libraries(game BomberBlob Box2D sfml-system sfml-graphics sfml-window -static-libgcc -static-libstdc++ -static -lpthread)

add_executable(Server Server.cpp)
target_link_libraries(Server sfml-network)

add_executable(Client Client.cpp)
target_link_libraries(Client sfml-network)
