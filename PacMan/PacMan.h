#pragma once
#include "Entity.h"

enum class ePacManState
{
	e_Normal,
	e_PowerUp
};

class PacMan final : public Entity
{
public:
	PacMan();

	void Update(const std::vector<std::vector<Tile>>& tiles);

	void Render(sf::RenderWindow& window);
	
	void PowerUp();
	void AddPoints(int amount);

private:
	int m_points;
	ePacManState m_state;
	float m_powerUpTimer;

	void Move();
};

