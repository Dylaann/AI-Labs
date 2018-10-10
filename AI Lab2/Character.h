#ifndef CHARACTER
#define CHARACTER

#include <SFML/Graphics.hpp>
#include <iostream>

class Player{
public:

	Player(float posX, float posY, sf::Texture &charTexture);

	void update();
	void draw(sf::RenderWindow & window);

	sf::Vector2f getPosition() { return m_position; }

private:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity = sf::Vector2f(0, 0);
	sf::Texture *m_CharTex;
	sf::Sprite m_sprite;

	float m_topSpeed = 8;
	float m_accel = 0.2;
	float m_currentSpeed = 0;
	float m_rotation;
	float m_rotationSpeed = 2;
	float m_rotRad;

	void checkWrapAround();
	void movePlayer();

};

class Ai{
public:

	Ai(float posX, float posY, sf::Texture &charTexture);

	void update(sf::Vector2f pos);
	void draw(sf::RenderWindow & window);

private:
	sf::Vector2f m_position;
	sf::Texture *m_CharTex;
	sf::Sprite m_sprite;
	sf::Vector2f m_velocity;

	float m_topSpeed = 2;
	float m_accel = 0.2;
	float m_currentSpeed = 0;
	float m_rotation;
	float m_rotationSpeed = 0.5;
	float m_rotRad;
	float m_rotateTarget;

	void checkWrapAround();
	void wander();
	int m_wanderRadius = 400;
	int m_accuracyRad = 2;
	int m_randrad;

	sf::Vector2f m_wanderPos = sf::Vector2f(0, 0);

	void seek(sf::Vector2f pos);

	void arrive(sf::Vector2f pos);
	float m_timeToTarget = 60;
	float radius = 200;

	void flee(sf::Vector2f pos);

	sf::Clock m_clock;
};

#endif
