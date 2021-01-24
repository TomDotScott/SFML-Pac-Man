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

	const ePacManState& GetPacManState() const;
	
	void PowerUp();
	void AddPoints(int amount);
	int GetPoints() const;
	
	int GetLivesRemaining() const;
	void Reset();
	
	bool IsAlive() const;
	void SetIsAlive(bool alive);

private:
	int m_points;
	int m_lives;
	bool m_isAlive;
	ePacManState m_state;
	float m_powerUpTimer;

	void Move();
};

