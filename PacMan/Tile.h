#pragma once
#include <SFML/System/Vector2.hpp>

enum class eTileType
{
	e_Air = -1,
	e_Wall = 0,
	e_Coin = 1,
	e_PowerUp = 2
};

struct Tile
{
	Tile(const eTileType type, const sf::Vector2i position, const bool canCollide) :
		m_type(type),
		m_position(position),
		m_canCollide(canCollide)
	{
	}

	eTileType m_type;
	const sf::Vector2i m_position;
	bool m_canCollide;
};