#pragma once
#include "Entity.h"
#include "PacMan.h"
#include <list>
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
	e_House,
	e_Chase,
	e_Scatter,
	e_Frightened
};

class Ghost final : public Entity
{
public:
	explicit Ghost(sf::Vector2i position, eGhostType type, sf::Color colour, const PacMan& pacMan);
	void Update(std::vector<std::vector<Tile>>& tiles);
	void Render(sf::RenderWindow& window);

	void SetGhostState(eGhostState state);

private:
	const PacMan& m_pacMan;
	eGhostType m_type;
	eGhostState m_state;

	// The path-finding will be updated every 10 game ticks (once per second)
	int m_updateTicks;
	int m_currentCorner;

	std::stack<Tile*> m_path;
	std::vector<Tile*> m_openList;
	std::vector<Tile*> m_closedList;
	sf::Vector2i m_cornerPositions[4];

	Tile* GetLowestFCostNode(std::vector<Tile*>& list);
	int CalculateDistanceCost(Tile* a, Tile* b) const;
	void CalculatePath(Tile* endNode);
	std::vector<Tile*> GetNeighbourNodes(std::vector<std::vector<Tile>>& tiles, Tile* currentNode);
	void AStarPathFinding(std::vector<std::vector<Tile>>& tiles, sf::Vector2i startPosition, sf::Vector2i endPosition);
	void Move(std::vector<std::vector<Tile>>& tiles);
	void UpdatePathFinding(std::vector<std::vector<Tile>>& tiles);
	void ChaseModePathFinding(std::vector<std::vector<Tile>>& tiles);
	void ScatterModePathFinding(std::vector<std::vector<Tile>>& tiles);
};

