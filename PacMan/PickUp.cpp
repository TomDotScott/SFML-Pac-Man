#include "PickUp.h"

#include <SFML/Graphics/CircleShape.hpp>

#include "Constants.h"

PickUp::PickUp() :
	m_position(),
	m_visible(false),
	m_type(ePickUpType::e_Coin)
{

}

void PickUp::Render(sf::RenderWindow& window) const
{
	sf::CircleShape circle;
	switch (m_type)
	{
	case ePickUpType::e_Coin:
		circle.setFillColor({ 255, 255, 71 });
		circle.setRadius(5);
		break;
	case ePickUpType::e_PowerUp:
		circle.setFillColor({ 255, 255, 255 });
		circle.setRadius(10);
		break;
	case ePickUpType::e_Cherry: break;
	default:;
	}

	circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);

	circle.setPosition(
		{
			static_cast<float>(m_position.x) + static_cast<float>(constants::k_gridCellSize) / 2.f,
			static_cast<float>(m_position.y) + static_cast<float>(constants::k_gridCellSize) / 2.f
		}
	);
	window.draw(circle);
}

void PickUp::Initialise(const sf::Vector2i position, const ePickUpType type)
{
	m_position = position;
	m_type = type;
	m_visible = true;
}

void PickUp::CheckPacManCollisions(PacMan& pacman)
{
	if (m_position == pacman.GetPosition())
	{
		switch (m_type)
		{
		case ePickUpType::e_Coin:
			pacman.AddPoints(10);
			break;
		case ePickUpType::e_PowerUp:
			pacman.AddPoints(500);
			pacman.PowerUp();
			break;
		default:;
		}

		m_visible = false;
	}
}

const sf::Vector2i& PickUp::GetPosition() const
{
	return m_position;
}

bool PickUp::Visible() const
{
	return m_visible;
}
