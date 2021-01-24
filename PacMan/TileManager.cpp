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
	m_pickupLocations.clear();

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
				case eTileType::e_Coin:
				case eTileType::e_PowerUp:
					m_pickupLocations.emplace_back(tilePosition, tileType);
					row.emplace_back(eTileType::e_Path, tilePosition, false);
					break;
				case eTileType::e_WrapAroundPath:
				case eTileType::e_Path:
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
			case eTileType::e_Path:
			case eTileType::e_WrapAroundPath:
				rec.setFillColor({ 128, 128, 128 });
				break;
			case eTileType::e_Wall:
				rec.setFillColor({ 0, 0, 64 });
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

const std::vector<std::pair<sf::Vector2i, eTileType>>& TileManager::GetPickUpLocations() const
{
	return m_pickupLocations;
}

std::vector<std::vector<Tile>>& TileManager::GetLevelData()
{
	return m_levelData;
}
