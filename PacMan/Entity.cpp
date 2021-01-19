#include "Entity.h"
#include <iostream>

Entity::Entity() :
	m_speed(5.f),
	m_position(400, 300),
	m_currentDirection(eDirection::e_Right),
	m_shape(10)
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
	default:
		std::cout << "Unknown Movement direction" << std::endl;
		break;
	}
}

void Entity::Render(sf::RenderWindow& window)
{
	m_shape.setPosition(m_position);
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
	default:
		std::cout << "Unknown Movement direction" << std::endl;
		break;
	}
}
