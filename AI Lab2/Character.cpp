#include "Character.h"

Player::Player(float posX, float posY, sf::Texture & charTexture) :
	m_position(posX, posY),
	m_CharTex(&charTexture)
{
	m_sprite.setTexture(*m_CharTex); //set texture
	m_sprite.setPosition(m_position); //set pos
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2.0); //set Origin to center
	m_sprite.setScale(0.75, 0.75);
}

void Player::update()
{		
	movePlayer();

	m_sprite.setRotation(m_rotation);
	m_sprite.setPosition(m_position);

	checkWrapAround();
}

void Player::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Player::checkWrapAround()
{
	if (m_position.x - (m_sprite.getGlobalBounds().width / 2) > 1920)
	{
		m_position.x = 0 - (m_sprite.getGlobalBounds().width / 2);
	}
	else if (m_position.x + (m_sprite.getGlobalBounds().width / 2) < 0)
	{
		m_position.x = 1920 + (m_sprite.getGlobalBounds().width / 2);
	}

	if (m_position.y - (m_sprite.getGlobalBounds().height / 2) > 1080)
	{
		m_position.y = 0 - (m_sprite.getGlobalBounds().height / 2);
	}
	else if (m_position.y + (m_sprite.getGlobalBounds().height / 2) < 0)
	{
		m_position.y = 1080 + (m_sprite.getGlobalBounds().height / 2);
	}

	m_sprite.setPosition(m_position);
}

void Player::movePlayer()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_currentSpeed < m_topSpeed)
	{
		m_currentSpeed += m_accel;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_currentSpeed > 0)
	{
		m_currentSpeed -= m_accel;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_rotation > 0)
	{
		m_rotation -= m_rotationSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_rotation == 0)
	{
		m_rotation = 360;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_rotation < 360)
	{
		m_rotation += m_rotationSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_rotation == 360) {
		m_rotation = 0;
	}

	m_rotRad = m_rotation * (3.14159265359 / 180);

	m_velocity.x = cos(m_rotRad) * m_currentSpeed;
	m_velocity.y = sin(m_rotRad) * m_currentSpeed;

	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

Ai::Ai(float posX, float posY, sf::Texture & charTexture) :
	m_position(posX, posY),
	m_CharTex(&charTexture)
{
	m_sprite.setTexture(*m_CharTex); //set texture
	m_sprite.setPosition(m_position); //set pos
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2.0); //set Origin to center
	m_sprite.setScale(0.4, 0.4);

	m_clock.restart();
}

void Ai::update(sf::Vector2f pos)
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;

	//wander();
	//seek(pos);
	//arrive(pos);
	//flee(pos);
	checkWrapAround();
}

void Ai::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Ai::checkWrapAround()
{
	if (m_position.x - (m_sprite.getGlobalBounds().width/2) > 1920)
	{
		m_position.x = 0 - (m_sprite.getGlobalBounds().width/2);
	}
	else if (m_position.x + (m_sprite.getGlobalBounds().width/2) < 0)
	{
		m_position.x = 1920 + (m_sprite.getGlobalBounds().width/2);
	}

	if (m_position.y - (m_sprite.getGlobalBounds().height/2) > 1080)
	{
		m_position.y = 0 - (m_sprite.getGlobalBounds().height/2);
	}
	else if (m_position.y + (m_sprite.getGlobalBounds().height /2) < 0)
	{
		m_position.y = 1080 + (m_sprite.getGlobalBounds().height / 2);
	}

	m_sprite.setPosition(m_position);
}

void Ai::wander()
{ 
	if (m_position.x > m_wanderPos.x - m_accuracyRad && m_position.x < m_wanderPos.x + m_accuracyRad &&
		m_position.y > m_wanderPos.y - m_accuracyRad && m_position.y < m_wanderPos.y + m_accuracyRad  || m_wanderPos == sf::Vector2f(0,0))
	{
		srand(m_clock.getElapsedTime().asSeconds());
		m_randrad =  rand() %360;

		m_wanderPos.x = m_position.x + m_wanderRadius * cos(m_randrad);
		if (m_wanderPos.x > 1920)
			m_wanderPos.x = 1920 - m_sprite.getGlobalBounds().width;
		if (m_wanderPos.x < 0)
			m_wanderPos.x = m_sprite.getGlobalBounds().width;

		srand(m_clock.getElapsedTime().asSeconds());
		m_randrad = rand() % 360;

		m_wanderPos.y = m_position.y + m_wanderRadius * sin(m_randrad);
		if (m_wanderPos.y > 1080)
			m_wanderPos.y = 1080 - m_sprite.getGlobalBounds().height;
		if (m_wanderPos.y < 0)
			m_wanderPos.y = m_sprite.getGlobalBounds().height;
	}

	m_velocity = m_wanderPos - m_position;

	m_velocity = sf::Vector2f(m_velocity.x / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))),
		m_velocity.y / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))));  //Normalize Velocity

	m_velocity *= m_topSpeed;

	m_rotation = atan2(m_wanderPos.y - m_sprite.getPosition().y, m_wanderPos.x - m_sprite.getPosition().x) * (180 / 3.14159265359);

	/*if (m_rotateTarget > m_rotation) {
		m_rotation += m_rotationSpeed;
	}
	else if (m_rotateTarget < m_rotation) {
		m_rotation -= m_rotationSpeed;
	}*/

	m_sprite.setRotation(m_rotation);
}

void Ai::seek(sf::Vector2f pos)
{
	m_velocity = pos - m_position;

	m_velocity = sf::Vector2f(m_velocity.x / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))),
		m_velocity.y / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))));  //Normalize Velocity

	m_velocity *= m_topSpeed;

	m_rotation = atan2(pos.y - m_sprite.getPosition().y, pos.x - m_sprite.getPosition().x) * (180 / 3.14159265359);
	m_sprite.setRotation(m_rotation);
}

void Ai::arrive(sf::Vector2f pos)
{
	m_velocity = pos - m_position;

	float mag = sqrt((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

	if (mag < radius) {
		m_velocity = sf::Vector2f(0, 0);
	}
	else {
		m_velocity = m_velocity / m_timeToTarget;
		mag = sqrt((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

			if (mag > m_topSpeed)
			{
				m_velocity = sf::Vector2f(m_velocity.x / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))),
					m_velocity.y / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))));  //Normalize Velocity

				m_velocity *= m_topSpeed;
			}

		m_rotation = atan2(pos.y - m_sprite.getPosition().y, pos.x - m_sprite.getPosition().x) * (180 / 3.14159265359);
		m_sprite.setRotation(m_rotation);
	}

}

void Ai::flee(sf::Vector2f pos)
{
	m_velocity = m_position - pos;

	m_velocity = sf::Vector2f(m_velocity.x / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))),
		m_velocity.y / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))));  //Normalize Velocity

	m_velocity *= m_topSpeed;

	m_rotation = atan2(pos.y - m_sprite.getPosition().y, pos.x - m_sprite.getPosition().x) * (180 / 3.14159265359) - 180;
	m_sprite.setRotation(m_rotation);
}


