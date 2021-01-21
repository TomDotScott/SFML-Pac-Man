#pragma once
#include "Entity.h"
#include "PacMan.h"

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
	explicit Ghost(sf::Vector2i position, eGhostType type, const PacMan& pacMan);
	void Update(const std::vector<std::vector<Tile>>& tiles);
	void Render(sf::RenderWindow& window);

private:
	const PacMan& m_pacMan;
	eGhostType m_type;
	eGhostState m_state;
	
	void PathFind(const std::vector<std::vector<Tile>>& tiles);
};

