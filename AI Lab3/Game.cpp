#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ 1920, 1080, 32 }, "AI Lab3"},
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture

	player = new Player(800, 500, m_playerTex);
	aiArrive = new Ai(1800, 100, m_playerTex, 6, "arrive", m_ArialBlackfont);
	aiSeek = new Ai(1800, 400, m_playerTex, 2, "seek", m_ArialBlackfont);
	aiPersue = new Ai(1800, 900, m_playerTex, 3, "persue", m_ArialBlackfont);
	aiArrive2 = new Ai(1500, 900, m_playerTex, 1, "arrive", m_ArialBlackfont);
	aiWander = new Ai(200, 900, m_playerTex, 1, "wander", m_ArialBlackfont);

	aiCharacters = new std::vector<Ai*>;
	aiCharacters->push_back(aiArrive);
	aiCharacters->push_back(aiSeek);
	aiCharacters->push_back(aiPersue);
	aiCharacters->push_back(aiArrive2);
	aiCharacters->push_back(aiWander);
}


Game::~Game()
{
}


void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type) //user key press
		{
			if (sf::Keyboard::Escape == event.key.code)
			{
				m_exitGame = true;
			}
		}
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	player->update();
	aiArrive->update(player->getPosition(), player->getVelocity(), aiCharacters);
	aiSeek->update(player->getPosition(), player->getVelocity(), aiCharacters);
	aiArrive2->update(player->getPosition(), player->getVelocity(), aiCharacters);
	aiPersue->update(player->getPosition(), player->getVelocity(), aiCharacters);
	aiWander->update(player->getPosition(), player->getVelocity(), aiCharacters);

}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);

	aiArrive->draw(m_window);
	aiSeek->draw(m_window);
	aiArrive2->draw(m_window);
	aiPersue->draw(m_window);
	aiWander->draw(m_window);
	player->draw(m_window);

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_playerTex.loadFromFile("ASSETS\\IMAGES\\Player.png"))
	{
		std::cout << "problem loading player Image" << std::endl;
	}

	if (!m_botTex.loadFromFile("ASSETS\\IMAGES\\Sun.png"))
	{
		std::cout << "problem loading bot Image" << std::endl;
	}
}
