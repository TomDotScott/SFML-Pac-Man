#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Entity.h"
#include "PickUp.h"
#include "TileManager.h"

class Game
{
public:
	Game();

	void Input();
	void Update();
	void Render(sf::RenderWindow& window);

private:
	Entity m_pacMan;
	std::vector<PickUp> m_pickups;
	TileManager m_tileManager;
	sf::Text m_score;
};

