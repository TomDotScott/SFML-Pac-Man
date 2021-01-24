#include "PacMan.h"

#include <iostream>


#include "Constants.h"
#include "Helpers.h"

PacMan::PacMan() :
	Entity(
	       constants::k_pacManSpawnPosition,
	       constants::k_gridCellSize,
	       eDirection::e_None,
	       sf::Color::Yellow
	      ),
	m_points(0),
	m_lives(3),
	m_isAlive(true),
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

		if (m_powerUpTimer >= constants::k_pacManPowerUpTime)
		{
			m_state = ePacManState::e_Normal;
			m_powerUpTimer = 0.f;
		}
	}
	m_clock.restart();
}

void PacMan::Render(sf::RenderWindow& window)
{
	if (m_state == ePacManState::e_PowerUp)
	{
		// Interpolate between pacman's colour and the power-up
		// Colour based on the time
		const float normalisedTimer = m_powerUpTimer / static_cast<float>(constants::k_pacManPowerUpTime);

		const sf::Uint32 lerpedColour = helpers::interpolate(m_colour.toInteger(), sf::Color::White.toInteger(), normalisedTimer);

		m_shape.setFillColor(sf::Color(lerpedColour));
	} else
	{
		m_shape.setFillColor(sf::Color::Yellow);
	}

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

int PacMan::GetPoints() const
{
	return m_points;
}

int PacMan::GetLivesRemaining() const
{
	return m_lives;
}

void PacMan::Reset()
{
	m_position = constants::k_pacManSpawnPosition;
	m_isAlive = true;
}

bool PacMan::IsAlive() const
{
	return m_isAlive;
}

void PacMan::SetIsAlive(const bool alive)
{
	if(!alive)
	{
		m_lives -= 1;
		m_points -= 500;
		if (m_points < 0) m_points = 0;
	}
	m_isAlive = alive;
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
