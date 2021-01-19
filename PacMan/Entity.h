#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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
	Entity();
	void Update();
	void Render(sf::RenderWindow& window);

	void SetDirection(eDirection direction);
	
	eDirection GetDirection() const;

	sf::Vector2i GetPosition() const;
	void SetPosition(const sf::Vector2i position);

private:
	int m_speed;
	sf::Vector2i m_position;
	eDirection m_currentDirection;
	sf::RectangleShape m_shape;
};
