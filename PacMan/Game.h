#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"

class Game
{
public:
	Game();

	void Input();
	void Update();
	void Render(sf::RenderWindow& window);

private:
	Entity m_pacMan;
};

