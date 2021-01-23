#pragma once
#include <SFML/System/Vector2.hpp>

#include "Constants.h"

enum class eTileType
{
	e_Path = -1,
	e_Wall = 0,
	e_Coin = 1,
	e_PowerUp = 2,
	e_WrapAroundPath = 3
};

struct Tile
{
	Tile(const eTileType type, const sf::Vector2i position, const bool canCollide) :
		m_type(type),
		m_position(position),
		m_canCollide(canCollide),
		m_cameFromNode(nullptr),
		m_fCost(0),
		m_gCost(0),
		m_hCost(0)
	{
	}
	
	bool operator==(const Tile& tile) const
	{
		return m_type == tile.m_type && m_position == tile.m_position;
	}
	
	void CalculateFCost()
	{
		m_fCost = m_gCost - m_hCost;
	}
	
	eTileType m_type;
	sf::Vector2i m_position;
	bool m_canCollide;

	Tile* m_cameFromNode;
	int m_fCost;
	int m_gCost;
	int m_hCost;
};