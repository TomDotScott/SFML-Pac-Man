#include "Entity.h"
#include "Constants.h"
#include <iostream>

#include "Helpers.h"

Entity::Entity(const sf::Vector2i position, const int speed, const eDirection startingDirection, sf::Color colour) :
	m_position(position),
	m_speed(speed),
	m_currentDirection(startingDirection),
	m_shape({ constants::k_gridCellSize, constants::k_gridCellSize }),
	m_colour(colour),
	m_clock()
{
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

void Entity::CheckForBlockades(const std::vector<std::vector<Tile>>& tiles)
{
	if (helpers::is_in_range(m_position.x, 0, constants::k_screenSize - constants::k_gridCellSize) &&
		helpers::is_in_range(m_position.y, 0, constants::k_screenSize - constants::k_gridCellSize))
	{
		const int entityX = m_position.x / constants::k_gridCellSize;
		const int entityY = m_position.y / constants::k_gridCellSize;

		{
			const auto& currentTile = tiles[entityY - 1][entityX];
			if (currentTile.m_canCollide)
			{
				if (m_position.y <= currentTile.m_position.y + constants::k_gridCellSize)
				{
					m_position = { m_position.x, currentTile.m_position.y + constants::k_gridCellSize };
					m_limitedDirections.push_back(eDirection::e_Up);
				}
			}
		}

		{
			const auto& currentTile = tiles[entityY + 1][entityX];
			if (currentTile.m_canCollide)
			{
				if (m_position.y >= currentTile.m_position.y - constants::k_gridCellSize)
				{
					m_position = { m_position.x, currentTile.m_position.y - constants::k_gridCellSize };
					m_limitedDirections.push_back(eDirection::e_Down);
				}
			}
		}

		{
			const auto& currentTile = tiles[entityY][entityX - 1];
			if (currentTile.m_canCollide)
			{
				if (m_position.x >= currentTile.m_position.x + constants::k_gridCellSize)
				{
					m_position = { currentTile.m_position.x + constants::k_gridCellSize, m_position.y };
					m_limitedDirections.push_back(eDirection::e_Left);
				}
			}
		}

		{
			const auto& currentTile = tiles[entityY][entityX + 1];
			if (currentTile.m_canCollide)
			{
				if (m_position.x <= currentTile.m_position.x - constants::k_gridCellSize)
				{
					m_position = { currentTile.m_position.x - constants::k_gridCellSize, m_position.y };
					m_limitedDirections.push_back(eDirection::e_Right);
				}
			}
		}
	}
}

void Entity::WrapAround()
{
	if (m_position.x < 0)
	{
		m_position.x = constants::k_screenSize + m_position.x;
	} else if (m_position.x > constants::k_screenSize - constants::k_gridCellSize)
	{
		m_position.x = constants::k_screenSize - m_position.x;
	}
}
