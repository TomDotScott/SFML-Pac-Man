#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Entity.h"
#include "Tile.h"

class TileManager
{
public:
	TileManager() = default;

	bool LoadLevel(const std::string& filename);

	void Render(sf::RenderWindow& window);

	const std::vector<std::pair<sf::Vector2i, eTileType>>& GetPickUpLocations() const;
	std::vector<std::vector<Tile>>& GetLevelData();
private:
	std::vector<std::vector<Tile>> m_levelData;
	std::vector<std::pair<sf::Vector2i, eTileType>> m_pickupLocations;
};
