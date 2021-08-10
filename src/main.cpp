#include <iostream>

#include <Blob/Core/Exception.hpp>

#include <Menu.hpp>

using namespace Blob;

int main() {

    try {
        //Camera camera;

        Window window;

        (Menu(window));

    } catch (Exception &e) {
        std::cout << "Blob Error :" << std::endl << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << "Game Error :" << std::endl << e.what() << std::endl;
    }
    return 0;
}
