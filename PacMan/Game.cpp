#include "Game.h"
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

#include "Constants.h"

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

	m_ghosts.emplace_back(
		sf::Vector2i(
			constants::k_screenSize - 2 * constants::k_gridCellSize, 
			3 * constants::k_gridCellSize
		), 
		eGhostType::e_Blinky, 
		sf::Color::Red, 
		m_pacMan
	);
	
	m_ghosts.emplace_back(
		sf::Vector2i(
			constants::k_screenSize / 2, 
			constants::k_screenSize / 2 + constants::k_gridCellSize
		), 
		eGhostType::e_Pinky, 
		sf::Color::Magenta, 
		m_pacMan
	);
	
	// m_ghosts.emplace_back(sf::Vector2i(constants::k_screenSize / 2, constants::k_screenSize / 2), eGhostType::e_Inky, m_pacMan);

	m_ghosts.emplace_back(
		sf::Vector2i(
			constants::k_screenSize / 2, 
			constants::k_screenSize / 2 - 5 * constants::k_gridCellSize
		), 
		eGhostType::e_Clyde,
		sf::Color(255, 165, 0),
		m_pacMan
	);
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
	m_pacMan.Update(m_tileManager.GetLevelData());

	for(auto& ghost : m_ghosts)
	{
		ghost.Update(m_tileManager.GetLevelData());
	}
	
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

	for (auto& ghost : m_ghosts)
	{
		ghost.Render(window);
	}
}
