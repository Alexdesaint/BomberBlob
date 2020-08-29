#include <iostream>

#include <Blob/Exception.hpp>

#include <Menu.hpp>

int main() {

    try {
        Blob::Camera camera;

        Blob::Window window(camera, false);

        (Menu(window));

    } catch (Blob::Exception &e) {
        std::cout << "Game Error :" << std::endl << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << "Game Error :" << std::endl << e.what() << std::endl;
    }
    return 0;
}
