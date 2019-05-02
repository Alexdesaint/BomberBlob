#include <iostream>

#include <BomberBlob/BomberBlob.hpp>

int main() {

    try {
        Blob::GL::Graphic graphic(false);

        (BomberBlob(graphic));

    } catch (Blob::Exception &e) {
        std::cout << "Game Error :" << std::endl << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cout << "Game Error :" << std::endl << e.what() << std::endl;
    }
    return 0;
}
