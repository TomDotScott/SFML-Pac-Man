#include "Ghost.h"

#include <iostream>


#include "Constants.h"
#include "Helpers.h"

Ghost::Ghost(const sf::Vector2i position, const eGhostType type, const PacMan& pacMan) :
	Entity(position,
		constants::k_gridCellSize,
		eDirection::e_None),
	m_pacMan(pacMan),
	m_type(type),
	m_state(eGhostState::e_Chase)
{
}

void Ghost::Update(const std::vector<std::vector<Tile>>& tiles)
{
	PathFind(tiles);
	Move();
	m_limitedDirections.clear();
	m_clock.restart();
}

void Ghost::Render(sf::RenderWindow& window)
{
	switch (m_type)
	{
	case eGhostType::e_Blinky:
		m_shape.setFillColor(sf::Color::Red);
		break;
	case eGhostType::e_Pinky:
		m_shape.setFillColor(sf::Color::Magenta);
		break;
	case eGhostType::e_Inky:
		m_shape.setFillColor(sf::Color::Cyan);
		break;
	case eGhostType::e_Clyde:
		m_shape.setFillColor({ 255, 140, 0 });
		break;
	default:;
	}

	m_shape.setPosition(static_cast<sf::Vector2f>(m_position));

	window.draw(m_shape);
}

void Ghost::PathFind(const std::vector<std::vector<Tile>>& tiles)
{
	// Find the directions that the ghost CAN'T Travel in
	CheckForBlockades(tiles);

	switch (m_state)
	{
	case eGhostState::e_House:
		// Move up and down and wait to be released
		break;
	case eGhostState::e_Chase:
		switch (m_type)
		{
		case eGhostType::e_Blinky:
			// Chase pacman and try to get behind him
			break;
		case eGhostType::e_Pinky:
		{
			// Chase pacman and try to get in front of him
			{
				// if deltaX is less than 0, pacman is on the right
				// if deltaY is less than 0, pacman is below
				const sf::Vector2i dist = m_position - m_pacMan.GetPosition();
				if (dist.x < 0 && !helpers::is_in_vector(m_limitedDirections, eDirection::e_Right))
				{
					m_currentDirection = eDirection::e_Right;
				} else if (dist.x > 0 && !helpers::is_in_vector(m_limitedDirections, eDirection::e_Left))
				{
					m_currentDirection = eDirection::e_Left;
				} else if (dist.y < 0 && !helpers::is_in_vector(m_limitedDirections, eDirection::e_Down))
				{
					m_currentDirection = eDirection::e_Down;
				} else if (dist.y > 0 && !helpers::is_in_vector(m_limitedDirections, eDirection::e_Up))
				{
					m_currentDirection = eDirection::e_Up;
				}
			}
		}
		break;
		case eGhostType::e_Inky:
			// Patrol an area
			break;
		case eGhostType::e_Clyde:
			// Move randomly in one direction until he can't
			if (helpers::is_in_vector(m_limitedDirections, m_currentDirection) || m_currentDirection == eDirection::e_None)
			{
				const int rand = helpers::rand_range(1, 4);
				if (rand == 1) m_currentDirection = eDirection::e_Up;
				else if (rand == 2)	m_currentDirection = eDirection::e_Down;
				else if (rand == 3)	m_currentDirection = eDirection::e_Left;
				else m_currentDirection = eDirection::e_Right;
			}
			break;
		default:;
		}
		break;
	case eGhostState::e_Scatter:
		// Break away from PacMan and head to the corners
		switch (m_type)
		{
		case eGhostType::e_Blinky:
			// Top Right
			break;
		case eGhostType::e_Pinky:
			// Top Left
			break;
		case eGhostType::e_Inky:
			// Bottom Right
			break;
		case eGhostType::e_Clyde:
			// Bottom Left
			break;
		default:;
		}
		break;
	case eGhostState::e_Frightened:
		// Run away from PacMan, move wherever is furthest away
		break;
	default:;
	}
}
