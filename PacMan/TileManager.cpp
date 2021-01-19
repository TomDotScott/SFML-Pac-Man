#include "TileManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Constants.h"
#include "Helpers.h"

bool TileManager::LoadLevel(const std::string& filename)
{
	// Clear the level data if it exists
	m_levelData.clear();

	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cout << "Couldn't Open the File: " + filename + "\nCheck the location and try again" << std::endl;
		return false;
	}

	while (!file.eof())
	{
		for (int r = 0; r < constants::k_gridSize; ++r)
		{
			std::vector<Tile> row;

			std::string line;
			std::getline(file, line);
			if (!file.good())
			{
				break;
			}

			std::stringstream iss(line);
			for (int c = 0; c < constants::k_gridSize; ++c)
			{
				std::string val;
				std::getline(iss, val, ',');
				if (!iss.good())
				{
					break;
				}

				const auto tileIntFromCSV = atoi(val.c_str());

				const sf::Vector2i tilePosition{
					c * constants::k_gridCellSize,
					r * constants::k_gridCellSize
				};

				const auto tileType = static_cast<eTileType>(tileIntFromCSV);

				switch (tileType)
				{
				case eTileType::e_Wall:
					row.emplace_back(tileType, tilePosition, true);
					break;

				case eTileType::e_Air:
					row.emplace_back(tileType, tilePosition, false);
					break;
				}
			}
			m_levelData.emplace_back(row);
		}
	}
	file.close();

	return true;
}

void TileManager::Render(sf::RenderWindow& window)
{
	sf::RectangleShape rec({
		static_cast<float>(constants::k_gridCellSize),
		static_cast<float>(constants::k_gridCellSize)
		}
	);

	for (auto& row : m_levelData)
	{
		for (const auto& currentTile : row)
		{
			switch (currentTile.m_type)
			{
			case eTileType::e_Air:
				rec.setFillColor({ 128, 128, 128 });
				break;
			case eTileType::e_Wall:
				rec.setFillColor({ 0, 0, 139 });
				break;
			default:
				std::cout << "Unknown tile type" << std::endl;
				break;
			}

			rec.setPosition(static_cast<sf::Vector2f>(currentTile.m_position));
			window.draw(rec);
		}
	}
}

void TileManager::CheckEntityLevelCollisions(Entity& entity)
{
	if (helpers::is_in_range(entity.GetPosition().x, 0, constants::k_screenSize) &&
		helpers::is_in_range(entity.GetPosition().y, 0, constants::k_screenSize))
	{
		const int entityX = entity.GetPosition().x / constants::k_gridCellSize;
		const int entityY = entity.GetPosition().y / constants::k_gridCellSize;
		// Check collisions in the direction the entity is facing
		switch (entity.GetDirection())
		{
		case eDirection::e_None:
			break;
		case eDirection::e_Up:
		{
			const auto& currentTile = m_levelData[entityY - 1][entityX];
			if (currentTile.m_canCollide)
			{
				if (entity.GetPosition().y <= currentTile.m_position.y + constants::k_gridCellSize)
				{
					entity.SetPosition({ entity.GetPosition().x, currentTile.m_position.y + constants::k_gridCellSize });
					entity.SetDirection(eDirection::e_None);
				}
			}
		}
		break;
		case eDirection::e_Down:
		{
			const auto& currentTile = m_levelData[entityY + 1][entityX];
			if (currentTile.m_canCollide)
			{
				if (entity.GetPosition().y >= currentTile.m_position.y - constants::k_gridCellSize)
				{
					entity.SetPosition({ entity.GetPosition().x, currentTile.m_position.y - constants::k_gridCellSize });
					entity.SetDirection(eDirection::e_None);
				}
			}
		}
		break;
		case eDirection::e_Left:
		{
			const auto& currentTile = m_levelData[entityY][entityX - 1];
			if (currentTile.m_canCollide)
			{
				if (entity.GetPosition().x >= currentTile.m_position.x + constants::k_gridCellSize)
				{
					entity.SetPosition({ currentTile.m_position.x + constants::k_gridCellSize, entity.GetPosition().y });
					entity.SetDirection(eDirection::e_None);
				}
			}
		}
		break;
		case eDirection::e_Right:
			const auto& currentTile = m_levelData[entityY][entityX + 1];
			if (currentTile.m_canCollide)
			{
				if (entity.GetPosition().x <= currentTile.m_position.x - constants::k_gridCellSize)
				{
					entity.SetPosition({ currentTile.m_position.x - constants::k_gridCellSize, entity.GetPosition().y });
					entity.SetDirection(eDirection::e_None);
				}
			}
			break;
		}
	}
}
