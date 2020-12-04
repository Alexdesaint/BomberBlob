add_executable(Game
		main.cpp
		Game.cpp
		Menu.cpp)

target_link_libraries(Game Blob BlobJump BomberBlob BlobSurvive)

add_subdirectory(Core)
add_subdirectory(BlobJump)
add_subdirectory(BomberBlob)
add_subdirectory(BlobSurvive)
