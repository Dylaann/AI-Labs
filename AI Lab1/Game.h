#ifndef GAME
#define GAME

using namespace std;

#include <SFML/Graphics.hpp>
#include "Character.h"
#include "CharFactory.h"
#include <iostream>


class Game
{
public:
	Game();
	~Game();

	void run();

private:

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	void setupFontAndText();
	void setupSprite();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	bool m_exitGame; // control exiting game

	sf::Texture m_playerTex;
	sf::Texture m_botTex;

	Factory* factory;
	vector<Character*> characters;
};

#endif // !GAME


