#include "PacMan.h"

#include "Constants.h"

PacMan::PacMan() :
	Entity(
		{
			constants::k_gridCellSize,
			2 * constants::k_gridCellSize
		},
		constants::k_gridCellSize,
		eDirection::e_None
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
}

void PacMan::Render(sf::RenderWindow& window)
{
	m_shape.setFillColor(m_state == ePacManState::e_Normal ? sf::Color::Yellow : sf::Color::White);

	m_shape.setPosition(static_cast<sf::Vector2f>(m_position));
	window.draw(m_shape);
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
