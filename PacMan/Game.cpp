#include "Game.h"

#include <SFML/Window/Keyboard.hpp>


Game::Game() :
m_pacMan()
{
	
}

void Game::Input()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_pacMan.SetDirection(eDirection::e_Up);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_pacMan.SetDirection(eDirection::e_Down);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_pacMan.SetDirection(eDirection::e_Left);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_pacMan.SetDirection(eDirection::e_Right);
	}
}

void Game::Update()
{
	m_pacMan.Update();
}

void Game::Render(sf::RenderWindow& window)
{
	m_pacMan.Render(window);
}
