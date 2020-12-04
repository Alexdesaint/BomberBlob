#include <iostream>

#include <Blob/Core/Exception.hpp>

#include <Menu.hpp>

using namespace Blob;

int main() {

    try {
        Core::Camera camera;

        Core::Window window(camera, false);

        (Menu(window));

    } catch (Core::Exception &e) {
        std::cout << "Blob Error :" << std::endl << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << "Game Error :" << std::endl << e.what() << std::endl;
    }
    return 0;
}
