#pragma once
#include "Entity.h"
#include "PacMan.h"
#include <stack>

enum class eGhostType
{
	e_Blinky,
	e_Pinky,
	e_Inky,
	e_Clyde
};

enum class eGhostState
{
	e_Chase,
	e_Scatter,
	e_Frightened
};

class Ghost final : public Entity
{
public:
	explicit Ghost(std::vector<std::vector<Tile>>& grid, sf::Vector2i position, eGhostType type, sf::Color colour, PacMan& pacMan);
	void Update();
	void Render(sf::RenderWindow& window);

	const eGhostState& GetGhostState() const;
	void SetGhostState(eGhostState state);

private:
	PacMan& m_pacMan;
	eGhostType m_type;
	eGhostState m_state;
	float m_homeTimer;
	
	// The path-finding will be updated every 10 game ticks (once per second)
	int m_updateTicks;
	std::vector<std::vector<Tile>>& m_grid;
	int m_currentCorner;

	std::stack<Tile*> m_path;
	std::vector<Tile*> m_openList;
	std::vector<Tile*> m_closedList;
	sf::Vector2i m_cornerPositions[4];
	sf::Vector2i m_homePositions[4];

	Tile* GetLowestFCostNode(std::vector<Tile*>& list);
	int CalculateDistanceCost(Tile* a, Tile* b) const;
	void CalculatePath(Tile* endNode);
	std::vector<Tile*> GetNeighbourNodes(Tile* currentNode) const;
	void AStarPathFinding(sf::Vector2i startPosition, sf::Vector2i endPosition);
	void UpdatePathFinding();
	void ChaseModePathFinding();
	void Move();
	void CheckPacManCollisions();

};

