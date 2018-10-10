#include "Character.h"

Character::Character()
{
}

Character::~Character()
{
}

Player::Player(float posX, float posY, sf::Texture & charTexture) :
	m_position(posX, posY),
	m_CharTex(&charTexture)
{
	m_sprite.setTexture(*m_CharTex); //set texture
	m_sprite.setPosition(m_position); //set pos
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2.0); //set Origin to center

}

void Player::update()
{		
	movePlayer();
	m_sprite.move(m_currentSpeed.x, m_currentSpeed.y);
	m_position.y = m_sprite.getPosition().y;
	m_position.x = m_sprite.getPosition().x;

	m_sprite.setRotation((atan2(m_currentSpeed.x, -m_currentSpeed.y) / 3.14159265359) * 180);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_currentSpeed.y > -m_topSpeed)
	{
		m_sprite.move(0.f, -m_accel);
		m_position.y = m_sprite.getPosition().y;
		m_currentSpeed.y -= m_accel;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_currentSpeed.y < m_topSpeed)
	{
		m_sprite.move(0.f, m_accel);
		m_position.y = m_sprite.getPosition().y;
		m_currentSpeed.y += m_accel;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_currentSpeed.x > -m_topSpeed)
	{
		m_sprite.move(-m_accel, 0.f);
		m_position.x = m_sprite.getPosition().x;
		m_currentSpeed.x -= m_accel;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_currentSpeed.x < m_topSpeed)
	{
		m_sprite.move(m_accel, 0.f);
		m_position.x = m_sprite.getPosition().x;
		m_currentSpeed.x += m_accel;
	}
}

Ai::Ai(float posX, float posY, sf::Texture & charTexture) :
	m_position(posX, posY),
	m_CharTex(&charTexture)
{
	m_sprite.setTexture(*m_CharTex); //set texture
	m_sprite.setPosition(m_position); //set pos
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2.0); //set Origin to center

	m_velocity.x = rand() % 5;
	m_velocity.y = m_velocity.x;
}

void Ai::update()
{
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
	checkWrapAround();
}

void Ai::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Ai::checkWrapAround()
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


