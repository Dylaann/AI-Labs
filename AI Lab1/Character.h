#ifndef CHARACTER
#define CHARACTER

#include <SFML/Graphics.hpp>
#include <iostream>

class Character
{
public:
	Character();

	virtual void update() = 0;

	virtual void draw(sf::RenderWindow & window) = 0;

	virtual ~Character();

};

class Player : public Character {
public:

	Player(float posX, float posY, sf::Texture &charTexture);

	void update();

	void draw(sf::RenderWindow & window);

private:
	sf::Vector2f m_position;

	sf::Texture *m_CharTex;

	sf::Sprite m_sprite;

	float m_topSpeed = 8;

	float m_accel = 0.25;

	sf::Vector2f m_currentSpeed = sf::Vector2f(0,0);

	void checkWrapAround();

	void movePlayer();

};

class Ai : public Character {
public:

	Ai(float posX, float posY, sf::Texture &charTexture);

	void update();

	void draw(sf::RenderWindow & window);

private:
	sf::Vector2f m_position;

	sf::Texture *m_CharTex;

	sf::Sprite m_sprite;

	sf::Vector2f m_velocity;

	void checkWrapAround();
};

#endif
