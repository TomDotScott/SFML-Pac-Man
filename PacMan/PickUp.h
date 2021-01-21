#pragma once
#include <SFML/System/Vector2.hpp>
#include "PacMan.h"

enum class ePickUpType
{
	e_Coin = 1,
	e_PowerUp = 2,
	e_Cherry = 3
};

class PickUp
{
public:
	PickUp();
	
	// void Update();
	void Render(sf::RenderWindow& window) const;

	void Initialise(sf::Vector2i position, ePickUpType type);

	bool Visible() const;

	void CheckPacManCollisions(PacMan& pacman);

private:
	sf::Vector2i m_position;
	bool m_visible;
	ePickUpType m_type;
};
