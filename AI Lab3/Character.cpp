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

Ai::Ai(float posX, float posY, sf::Texture & charTexture, float maxSpeed, std::string behaviour, sf::Font & font) :
	m_position(posX, posY),
	m_CharTex(&charTexture),
	m_topSpeed(maxSpeed),
	m_behaviour(behaviour)
{
	srand(m_clock.getElapsedTime().asSeconds());
	m_sprite.setTexture(*m_CharTex); //set texture
	m_sprite.setPosition(m_position); //set pos
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2.0); //set Origin to center
	m_sprite.setScale(0.4, 0.4);

	radius = m_sprite.getGlobalBounds().width;

	m_clock.restart();

	m_name.setFont(font);
	m_name.setString(m_behaviour);
	m_name.setCharacterSize(40);
	m_name.setPosition(sf::Vector2f(m_sprite.getPosition().x - 40, m_sprite.getPosition().y - 100));
	m_name.setFillColor(sf::Color::Blue);
}

void Ai::update(sf::Vector2f pos, sf::Vector2f vel, std::vector<Ai*>* aiChars)
{
	m_aiChars = aiChars;

	if (m_behaviour == "wander")
		wander(pos);
	if (m_behaviour == "seek")
		seek(pos);
	if (m_behaviour == "arrive")
		arrive(pos);
	if (m_behaviour == "flee")
		flee(pos);
	if (m_behaviour == "persue")
		persue(pos, vel);

	checkWrapAround();
	coneOfVision();
	collisionAvoid(m_aiChars);

	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;

	m_name.setPosition(sf::Vector2f(m_sprite.getPosition().x - 40, m_sprite.getPosition().y - 100));
}

void Ai::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
	window.draw(m_cone);
	window.draw(m_name);
}

void Ai::checkWrapAround()
{
	if (m_position.x - (m_sprite.getGlobalBounds().width / 2) > 1920)
	{
		m_position.x = 0 - (m_sprite.getGlobalBounds().width / 2);
		m_wanderPos.x = m_wanderPos.x - 1920;
	}
	else if (m_position.x + (m_sprite.getGlobalBounds().width / 2) < 0)
	{
		m_position.x = 1920 + (m_sprite.getGlobalBounds().width / 2);
		m_wanderPos.x = m_wanderPos.x + 1920;
	}

	if (m_position.y - (m_sprite.getGlobalBounds().height /2) > 1080)
	{
		m_position.y = 0 - (m_sprite.getGlobalBounds().height / 2);
		m_wanderPos.y = m_wanderPos.y - 1080;
	}
	else if (m_position.y + (m_sprite.getGlobalBounds().height / 2) < 0)
	{
		m_position.y = 1080 + (m_sprite.getGlobalBounds().height / 2);
		m_wanderPos.y = m_wanderPos.y + 1080;
	}

	m_sprite.setPosition(m_position);
}

void Ai::wander(sf::Vector2f pos)
{ 
	if (m_position.x > m_wanderPos.x - m_accuracyRad && m_position.x < m_wanderPos.x + m_accuracyRad &&
		m_position.y > m_wanderPos.y - m_accuracyRad && m_position.y < m_wanderPos.y + m_accuracyRad  || m_wanderPos == sf::Vector2f(0,0))
	{
		m_rotation = atan2(pos.y - m_sprite.getPosition().y, pos.x - m_sprite.getPosition().x) * (180 / 3.14159265359);

		float rotInRad = m_rotation * (3.14159265359 / 180);

		m_randrad =  rand() % 181 + (-90);
		m_randrad = m_randrad * (3.14159265359 / 180);

		m_wanderPos.x = m_position.x + m_wanderRadius * cos(rotInRad + m_randrad);

		m_randrad = rand() % 181 + (-90);
		m_randrad = m_randrad * (3.14159265359 / 180);

		m_wanderPos.y = m_position.y + m_wanderRadius * sin(rotInRad + m_randrad);
	}

	m_velocity = m_wanderPos - m_position;

	m_velocity = sf::Vector2f(m_velocity.x / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))),
		m_velocity.y / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))));  //Normalize Velocity

	m_velocity *= m_topSpeed;

	m_rotation = atan2(m_wanderPos.y - m_sprite.getPosition().y, m_wanderPos.x - m_sprite.getPosition().x) * (180 / 3.14159265359);

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
	float dist = sqrt((m_position.x - pos.x) * (m_position.x - pos.x) + (m_position.y - pos.y) * (m_position.y - pos.y));
	m_velocity = pos - m_position;

	float mag = sqrt((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

	if (mag < radius) {
		m_velocity = sf::Vector2f(0, 0);
	}
	else {

		if (dist < outerRadius) {
			m_velocity = m_velocity / m_timeToTarget;
			mag = sqrt((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

			if (mag > m_topSpeed)
			{
				m_velocity = sf::Vector2f(m_velocity.x / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))),
					m_velocity.y / sqrt(((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y))));  //Normalize Velocity

				m_velocity *= m_topSpeed;
			}
		}
		else
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

void Ai::persue(sf::Vector2f pos, sf::Vector2f vel)
{
	float timepred;

	sf::Vector2f direction = pos - m_position;

	float distance = sqrt((direction.x * direction.x) + (direction.y * direction.y));

	float speed = sqrt((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

	if (speed <= distance / maxTimePred)
	{
		timepred = maxTimePred;
	}
	else
	{
		timepred = distance / speed;
	}

	sf::Vector2f newPos = pos + vel * timepred;

	seek(newPos);

}

void Ai::coneOfVision()
{
	float rotInRad = m_rotation * (3.14159265359 / 180);
	float angleInRad = angle * (3.14159265359 / 180);

	float radLeft = rotInRad - angleInRad;
	float radRight = rotInRad + angleInRad;

	sf::Vector2f center = m_position;

	sf::Vector2f left = sf::Vector2f(center.x + coneLen * cos(radLeft), center.y + coneLen * sin(radLeft));
	sf::Vector2f right = sf::Vector2f(center.x + coneLen * cos(radRight), center.y + coneLen * sin(radRight));


	m_cone.setPointCount(3);

	m_cone.setPoint(0, center);
	m_cone.setPoint(1, left);
	m_cone.setPoint(2, right);

	m_cone.setFillColor(sf::Color(255, 255, 255, 128));
}

float Ai::sign(sf::Vector2f point, sf::Vector2f point1, sf::Vector2f point2)
{
	return (point.x - point2.x) * (point1.y - point2.y) - (point1.x - point2.x) * (point.y - point2.y);
}

bool Ai::PointInTriangle(sf::Vector2f point, sf::ConvexShape cone)
{
	bool b1, b2, b3;

	b1 = sign(point, cone.getPoint(0), cone.getPoint(1)) < 0.0f;
	b2 = sign(point, cone.getPoint(1), cone.getPoint(2)) < 0.0f;
	b3 = sign(point, cone.getPoint(2), cone.getPoint(0)) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

void Ai::collisionAvoid(std::vector<Ai*>* aiChars)
{
	for (Ai* target : *m_aiChars) {
		if (target->m_position != m_position) {

			sf::Vector2f direction = target->m_position - m_position;

			float mag = sqrt((direction.x * direction.x) + (direction.y *direction.y));

			float strength;

			if (PointInTriangle(target->m_position, m_cone))
			{
				m_cone.setFillColor(sf::Color(0, 186, 0, 128));
			}

			if (PointInTriangle(target->m_position, m_cone) && mag < radius)
			{
				strength = fmin(2 * mag * mag, m_topSpeed);

				direction = sf::Vector2f(direction.x / sqrt(((direction.x *direction.x) + (direction.y * direction.y))),
					direction.y / sqrt(((direction.x * direction.x) + (direction.y * direction.y))));  //Normalize Velocity

				seek(direction * strength);
			}
		}
	}
}



