#ifndef BOMBERMAN_GAMEMENU_HPP
#define BOMBERMAN_GAMEMENU_HPP

#include <BlobEngine/BlobGL/Graphic.hpp>

class GameMenu {
private:
	Blob::GL::Graphic &window;

public:
	explicit GameMenu(Blob::GL::Graphic &window);
};


#endif //BOMBERMAN_GAMEMENU_HPP
