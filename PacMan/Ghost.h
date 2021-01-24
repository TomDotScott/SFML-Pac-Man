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
	explicit Ghost(eGhostType type, std::vector<std::vector<Tile>>& grid, PacMan& pacMan);
	void Update();
	void Render(sf::RenderWindow& window);

	void Reset();
	
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

