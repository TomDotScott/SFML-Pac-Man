#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Tile.h"

enum class eDirection
{
	e_None,
	e_Up,
	e_Down,
	e_Left,
	e_Right
};

inline const char* to_string(eDirection e)
{
	switch (e)
	{
	case eDirection::e_None: return "e_None";
	case eDirection::e_Up: return "e_Up";
	case eDirection::e_Down: return "e_Down";
	case eDirection::e_Left: return "e_Left";
	case eDirection::e_Right: return "e_Right";
	default: return "unknown";
	}
}

class Entity
{
public:
	void SetDirection(eDirection direction);
	eDirection GetDirection() const;

	sf::Vector2i GetPosition() const;
	void SetPosition(const sf::Vector2i position);

protected:
	sf::Vector2i m_position;
	int m_speed;
	eDirection m_currentDirection;
	std::vector<eDirection> m_limitedDirections;
	sf::RectangleShape m_shape;
	sf::Clock m_clock;


	Entity(sf::Vector2i position, int speed, eDirection startingDirection);

	void Move();
	void CheckForBlockades(const std::vector<std::vector<Tile>>& tiles);

private:

	void WrapAround();	
};
