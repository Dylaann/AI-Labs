#ifndef CHARFACTORY_H
#define CHARFACTORY_H

#include "Character.h"

class Factory {
public:
	virtual Character* CreatePlayer(float posX, float posY, sf::Texture & charTexture) = 0;
	virtual Character* CreateAi(float posX, float posY, sf::Texture & charTexture) = 0;
};

class CharFactory : public Factory {
public:
	Character * CreatePlayer(float posX, float posY, sf::Texture & charTexture) {
		return new Player(posX, posY, charTexture);
	}

	Character * CreateAi(float posX, float posY, sf::Texture & charTexture) {
		return new Ai(posX, posY, charTexture);
	}
};

#endif // !FACTORY_H
