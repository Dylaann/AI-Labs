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
	sf::Vector2f getVelocity() { return m_velocity; }

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

	Ai(float posX, float posY, sf::Texture &charTexture, float maxSpeed, std::string behaviour, sf::Font & font);

	void update(sf::Vector2f pos, sf::Vector2f vel, std::vector<Ai*>* aiChars);
	void draw(sf::RenderWindow & window);

private:
	sf::Vector2f m_position;
	sf::Texture *m_CharTex;
	sf::Sprite m_sprite;
	sf::Vector2f m_velocity;
	std::string m_behaviour;
	sf::Text m_name;

	float m_topSpeed;
	float m_accel = 0.2;
	float m_currentSpeed = 0;
	float m_rotation;
	float m_rotationSpeed = 0.5;
	float m_rotRad;
	float m_rotateTarget;

	void checkWrapAround();
	void wander(sf::Vector2f pos);
	int m_wanderRadius = 150;
	int m_accuracyRad = 2;
	int m_randrad;

	sf::Vector2f m_wanderPos = sf::Vector2f(0, 0);

	void seek(sf::Vector2f pos);

	void arrive(sf::Vector2f pos);
	float m_timeToTarget = 200;
	float radius;
	float outerRadius = 500;

	void flee(sf::Vector2f pos);

	void persue(sf::Vector2f pos, sf::Vector2f vel);
	float maxTimePred = 40;

	void coneOfVision();
	float sign(sf::Vector2f point, sf::Vector2f point1, sf::Vector2f point2);
	bool PointInTriangle(sf::Vector2f point, sf::ConvexShape cone);
	float angle = 45;
	float coneLen = 400;
	sf::ConvexShape m_cone;

	void collisionAvoid(std::vector<Ai*>* aiChars);
	std::vector<Ai*>* m_aiChars;

	sf::Clock m_clock;
};

#endif
