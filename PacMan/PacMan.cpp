#include "PacMan.h"

#include <iostream>


#include "Constants.h"
#include "Helpers.h"

PacMan::PacMan() :
	Entity(
		{
			constants::k_gridCellSize,
			2 * constants::k_gridCellSize
		},
		constants::k_gridCellSize,
		eDirection::e_None,
		sf::Color::Yellow
	),
	m_points(0),
	m_state(ePacManState::e_Normal),
	m_powerUpTimer(0.f)
{
}

void PacMan::Update(const std::vector<std::vector<Tile>>& tiles)
{
	CheckForBlockades(tiles);
	Move();
	if (m_state == ePacManState::e_PowerUp)
	{
		m_powerUpTimer += static_cast<float>(m_clock.getElapsedTime().asSeconds());

		if (m_powerUpTimer >= 5.f)
		{
			m_state = ePacManState::e_Normal;
			m_powerUpTimer = 0.f;
		}
	}
	m_clock.restart();

	std::cout << m_position.x << " " << m_position.y << std::endl;
}

void PacMan::Render(sf::RenderWindow& window)
{
	m_shape.setFillColor(m_state == ePacManState::e_Normal ? sf::Color::Yellow : sf::Color::White);

	m_shape.setPosition(static_cast<sf::Vector2f>(m_position));
	window.draw(m_shape);
}

const ePacManState& PacMan::GetPacManState() const
{
	return m_state;
}

void PacMan::PowerUp()
{
	m_state = ePacManState::e_PowerUp;
	m_powerUpTimer = 0.f;
}

void PacMan::AddPoints(const int amount)
{
	m_points += amount;
}

void PacMan::Move()
{
	if (!helpers::is_in_vector(m_limitedDirections, m_currentDirection))
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
		WrapAround();
	}
	m_limitedDirections.clear();
}
