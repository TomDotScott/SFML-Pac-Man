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

	void CheckEntityLevelCollisions(Entity& entity);

private:
	std::vector<std::vector<Tile>> m_levelData;
};
