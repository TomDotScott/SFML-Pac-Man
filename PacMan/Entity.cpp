#include "Entity.h"
#include "Constants.h"
#include <iostream>

Entity::Entity() :
	m_speed(constants::k_gridCellSize),
	m_position(
		constants::k_screenSize / 2 + 3 * constants::k_gridCellSize,
		constants::k_screenSize / 2 - 3 * constants::k_gridCellSize
		),
	m_currentDirection(eDirection::e_Up),
	m_shape({constants::k_gridCellSize, constants::k_gridCellSize})
{
	m_shape.setFillColor(sf::Color::Yellow);
}

void Entity::Update()
{
	switch (m_currentDirection)
	{
	case eDirection::e_Up:
		m_position.y -= m_speed;
		break;
	case eDirection::e_Down:
		m_position.y += m_speed;
		break;
	case eDirection::e_Left:
		m_position.x -= m_speed;
		break;
	case eDirection::e_Right:
		m_position.x += m_speed;
		break;
	case eDirection::e_None:
		break;
	default:
		std::cout << "Unknown Movement direction" << std::endl;
		break;
	}
}

void Entity::Render(sf::RenderWindow& window)
{
	m_shape.setPosition(static_cast<sf::Vector2f>(m_position));
	window.draw(m_shape);
}

void Entity::SetDirection(const eDirection direction)
{
	// can't go back on himself
	switch (direction)
	{
	case eDirection::e_Up:
		if (m_currentDirection != eDirection::e_Down)
		{
			m_currentDirection = direction;
		}
		break;
	case eDirection::e_Down:
		if (m_currentDirection != eDirection::e_Up)
		{
			m_currentDirection = direction;
		}
		break;
	case eDirection::e_Left:
		if (m_currentDirection != eDirection::e_Right)
		{
			m_currentDirection = direction;
		}
		break;
	case eDirection::e_Right:
		if (m_currentDirection != eDirection::e_Left)
		{
			m_currentDirection = direction;
		}
		break;
	case eDirection::e_None:
		m_currentDirection = direction;
		break;
	default:
		std::cout << "Unknown Movement direction" << std::endl;
		break;
	}
}

eDirection Entity::GetDirection() const
{
	return m_currentDirection;
}

sf::Vector2i Entity::GetPosition() const
{
	return m_position;
}

void Entity::SetPosition(const sf::Vector2i position)
{
	m_position = position;
}
