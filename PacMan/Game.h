#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Entity.h"
#include "Ghost.h"
#include "PacMan.h"
#include "PickUp.h"
#include "Text.h"
#include "TileManager.h"

class Game
{
public:
	Game();

	void Input();
	void Update();
	void Render(sf::RenderWindow& window);

private:
	bool m_gameOver;
	PacMan m_pacMan;
	std::vector<PickUp> m_pickups;
	std::vector<Ghost> m_ghosts;
	TileManager m_tileManager;
	
	Text m_score;
	Text m_lives;
	Text m_end;
	
	sf::Font m_font;

	void SpawnNewPowerUp();
};