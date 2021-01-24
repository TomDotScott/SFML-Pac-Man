#pragma once
namespace constants
{
	const int k_screenSize = 800;
	const int k_gridSize = 32;
	const int k_gridCellSize = k_screenSize / k_gridSize;
	const int k_gridMovementCost = 10;
	const int k_ghostHomeTime = 7;
	const int k_pacManPowerUpTime = 5;

	const sf::Vector2i k_pacManSpawnPosition = { 375, 375 };
	
	const sf::Vector2i k_cornerPositions[]
	{
		{ constants::k_gridCellSize, 2 * constants::k_gridCellSize },
		{ 30 * constants::k_gridCellSize, 2 * constants::k_gridCellSize },
		{ 30 * constants::k_gridCellSize, 29 * constants::k_gridCellSize },
		{ constants::k_gridCellSize, 29 * constants::k_gridCellSize }
	};

	const sf::Vector2i k_homePositions[]
	{
		{ 350, 375 },
		{ 375, 375 },
		{ 400, 375 },
		{ 425, 375 }
	};
}