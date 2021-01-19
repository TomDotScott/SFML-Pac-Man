#include "Game.h"
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

Game::Game() :
	m_pacMan()
{
	if (!m_tileManager.LoadLevel("Data\\Level.csv"))
	{
		std::cout << "Error loading level data" << std::endl;
	}

	for (const auto& pickup : m_tileManager.GetPickUpLocations())
	{
		m_pickups.emplace_back();
		m_pickups.back().Initialise(pickup.first, static_cast<ePickUpType>(pickup.second));
	}
}

void Game::Input()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
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
	m_tileManager.CheckEntityLevelCollisions(m_pacMan);
	m_pacMan.Update();

	for (auto& pickup : m_pickups)
	{
		if(pickup.Visible())
		{
			pickup.CheckPacManCollisions(m_pacMan);
		}
	}
}

void Game::Render(sf::RenderWindow& window)
{
	m_tileManager.Render(window);
	
	for(const auto& pickup : m_pickups)
	{
		if (pickup.Visible())
		{
			pickup.Render(window);
		}
	}
	
	m_pacMan.Render(window);
}
