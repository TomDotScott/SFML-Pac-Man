#include "Game.h"
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

#include "Constants.h"
#include "Helpers.h"

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
		m_tileManager.GetLevelData(),
		sf::Vector2i(
			constants::k_screenSize - 2 * constants::k_gridCellSize, 
			3 * constants::k_gridCellSize
		), 
		eGhostType::e_Blinky, 
		sf::Color::Red, 
		m_pacMan
	);
	
	m_ghosts.emplace_back(
		m_tileManager.GetLevelData(),
		sf::Vector2i(
			constants::k_screenSize / 2, 
			constants::k_screenSize / 2 + constants::k_gridCellSize
		), 
		eGhostType::e_Pinky, 
		sf::Color::Magenta, 
		m_pacMan
	);
	
	m_ghosts.emplace_back(
		m_tileManager.GetLevelData(),
		sf::Vector2i(
			constants::k_screenSize / 2, 
			constants::k_screenSize / 2
		), 
		eGhostType::e_Inky,
		sf::Color::Cyan,
		m_pacMan
	);

	m_ghosts.emplace_back(
		m_tileManager.GetLevelData(),
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

	for (auto& pickup : m_pickups)
	{
		if (pickup.Visible())
		{
			pickup.CheckPacManCollisions(m_pacMan);
		}
	}
	
	for(auto& ghost : m_ghosts)
	{
		if (ghost.GetGhostState() != eGhostState::e_Frightened)
		{
			switch (m_pacMan.GetPacManState())
			{
			case ePacManState::e_Normal:
				ghost.SetGhostState(eGhostState::e_Chase);
				break;
			case ePacManState::e_PowerUp:
				ghost.SetGhostState(eGhostState::e_Scatter);
				break;
			default:;
			}
		}
		ghost.Update();
	}

	if(helpers::rand_range(0, 1000) <= 5)
	{
		SpawnNewPowerUp();
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

void Game::SpawnNewPowerUp()
{
	// Find an appropriate place to spawn the new power-up
	auto& map = m_tileManager.GetLevelData();
	Tile& randomTile = map[1][1];
	PickUp* firstAvailablePickup = nullptr;

	bool tileTaken = false;
	do
	{
		const sf::Vector2i random(helpers::rand_range(25, 750), helpers::rand_range(50, 725));
		randomTile = map[helpers::world_coord_to_array_index(random.y)][helpers::world_coord_to_array_index(random.y)];

		// See if there is already a coin or pickup at this position
		for (auto& pickup : m_pickups)
		{
			if(pickup.Visible() && pickup.GetPosition() == randomTile.m_position) tileTaken = true;
			else
			{
				if (!pickup.Visible() && !firstAvailablePickup)
				{
					firstAvailablePickup = &pickup;
				}
			}
		}
	} while (randomTile.m_type != eTileType::e_Path && !tileTaken);

	if (firstAvailablePickup)
	{
		firstAvailablePickup->Initialise(randomTile.m_position, ePickUpType::e_PowerUp);
	}
}
