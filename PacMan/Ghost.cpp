#include "Ghost.h"

#include <iostream>


#include "Constants.h"
#include "Helpers.h"

Ghost::Ghost(const sf::Vector2i position, const eGhostType type, const sf::Color colour, const PacMan& pacMan) :
	Entity(position,
		constants::k_gridCellSize,
		eDirection::e_None,
		colour),
	m_pacMan(pacMan),
	m_type(type),
	m_state(eGhostState::e_Chase),
	m_updateTicks(0),
	m_currentCorner(0)
{
	// define the corners of the map for patrolling
	// and scattering
	m_cornerPositions[0] = { constants::k_gridCellSize, 2 * constants::k_gridCellSize };
	m_cornerPositions[1] = { 30 * constants::k_gridCellSize, 2 * constants::k_gridCellSize };
	m_cornerPositions[2] = { 30 * constants::k_gridCellSize, 29 * constants::k_gridCellSize };
	m_cornerPositions[3] = { constants::k_gridCellSize, 29 * constants::k_gridCellSize };
}

void Ghost::Update(std::vector<std::vector<Tile>>& tiles)
{
	Move(tiles);

	// m_limitedDirections.clear();

	m_clock.restart();
}

void Ghost::Render(sf::RenderWindow& window)
{
	std::stack<Tile*> temp = m_path;

	while (!temp.empty())
	{
		auto* node = temp.top();
		temp.pop();

		m_shape.setFillColor({ m_colour.r, m_colour.g, m_colour.b, 80 });
		m_shape.setPosition(static_cast<sf::Vector2f>(node->m_position));
		window.draw(m_shape);
	}


	m_shape.setFillColor(m_colour);
	m_shape.setPosition(static_cast<sf::Vector2f>(m_position));

	window.draw(m_shape);
}

void Ghost::SetGhostState(const eGhostState state)
{
	m_state = state;
}

Tile* Ghost::GetLowestFCostNode(std::vector<Tile*>& list)
{
	if (!list.empty())
	{
		Tile* lowestFCostNode = list[0];

		for (auto& node : list)
		{
			// overwrite the lowest Node if the F-Cost is lower
			if (node->m_fCost < lowestFCostNode->m_fCost)
			{
				lowestFCostNode = node;
			}
		}

		return lowestFCostNode;
	}
	return nullptr;
}

int Ghost::CalculateDistanceCost(Tile* a, Tile* b) const
{
	const int deltaX = abs(a->m_position.x - b->m_position.x);
	const int deltaY = abs(a->m_position.y - b->m_position.y);

	const int remaining = abs(deltaX - deltaY);

	return 15 * (deltaX < deltaY ? deltaX : deltaY) + constants::k_gridMovementCost * remaining;
}

void Ghost::CalculatePath(Tile* endNode)
{
	// clear the stack
	while (!m_path.empty())
	{
		m_path.pop();
	}

	m_path.push(endNode);

	Tile* currentNode = endNode;

	// Cycle through the parents until we find a node with no parent
	// this node is the start node
	while (currentNode->m_cameFromNode != nullptr)
	{
		m_path.push(currentNode->m_cameFromNode);
		currentNode = currentNode->m_cameFromNode;
	}

	if (m_path.empty())
	{
		std::cout << "No path to X: " << endNode->m_position.x
			<< " Y : " << endNode->m_position.y << " found" << std::endl;
	}
}

std::vector<Tile*> Ghost::GetNeighbourNodes(std::vector<std::vector<Tile>>& tiles, Tile* currentNode)
{
	std::vector<Tile*> neighbours;

	const int xIndex = helpers::world_coord_to_array_index(currentNode->m_position.x);
	const int yIndex = helpers::world_coord_to_array_index(currentNode->m_position.y);

	// Find the 4 neighbour positions if they are valid
	if (xIndex - 1 >= 0)
	{
		// Left
		neighbours.push_back(&tiles[yIndex][xIndex - 1]);
	}

	if (xIndex + 1 < static_cast<int>(tiles[0].size()))
	{
		// Right
		neighbours.push_back(&tiles[yIndex][xIndex + 1]);
	}

	// Top
	if (yIndex - 1 >= 0)
	{
		neighbours.push_back(&tiles[yIndex - 1][xIndex]);
	}

	// Bottom
	if (yIndex + 1 < static_cast<int>(tiles.size()))
	{
		neighbours.push_back(&tiles[yIndex + 1][xIndex]);
	}

	return neighbours;
}

void Ghost::AStarPathFinding(std::vector<std::vector<Tile>>& tiles, const sf::Vector2i startPosition, const sf::Vector2i endPosition)
{
	// Calculate the array indices of the start and end positions
	const sf::Vector2i startNodeIndices(
		startPosition.x / constants::k_gridCellSize,
		startPosition.y / constants::k_gridCellSize
	);

	const sf::Vector2i endNodeIndices(
		endPosition.x / constants::k_gridCellSize,
		endPosition.y / constants::k_gridCellSize
	);

	Tile* startNode = &tiles[startNodeIndices.y][startNodeIndices.x];
	Tile* endNode = &tiles[endNodeIndices.y][endNodeIndices.x];

	m_openList.clear();
	m_closedList.clear();

	// Cycle through grid, set g to infinite and calculate f cost
	for (auto& tile : tiles)
	{
		for (auto& pathNode : tile)
		{
			// get a reference to the object
			// set g cost to infinite
			pathNode.m_gCost = INT_MAX;

			// calculate f cost
			pathNode.CalculateFCost();
			pathNode.m_cameFromNode = nullptr;
		}
	}

	// Calculate the costs of the start node
	startNode->m_gCost = 0;
	startNode->m_hCost = CalculateDistanceCost(startNode, endNode);
	startNode->CalculateFCost();
	m_openList.push_back(startNode);
	int loops = 0;
	// Cycle through all of the nodes and find a path
	while (!m_openList.empty())
	{
		++loops;
		// The current node is the node in the open list with the lowest F-Cost
		Tile* currentNode = GetLowestFCostNode(m_openList);

		m_openList.erase(std::remove(m_openList.begin(), m_openList.end(), currentNode));
		m_closedList.push_back(currentNode);

		// If the currentNode is the end
		if (currentNode == endNode)
		{
			CalculatePath(endNode);
		}


		for (auto& neighbour : GetNeighbourNodes(tiles, currentNode))
		{
			// Make sure the current neighbour node isn't in the closed list
			if (helpers::is_in_vector(m_closedList, neighbour))
			{
				continue;
			}

			// If the neighbour blocks the path
			if (neighbour->m_type == eTileType::e_Wall)
			{
				// add to the closed list
				m_closedList.push_back(neighbour);
				// continue to the beginning of the For Loop
				continue;
			}

			// Update the node
			neighbour->m_cameFromNode = currentNode;
			neighbour->m_gCost = currentNode->m_gCost + CalculateDistanceCost(currentNode, neighbour);

			// Calculate the new H-Cost for the node
			neighbour->m_hCost = CalculateDistanceCost(neighbour, endNode);
			// neighbour.CalculateFCost();
			neighbour->m_fCost = neighbour->m_gCost + neighbour->m_hCost;

			// if the open list doesn't contain the neighbour, add it
			if (helpers::is_in_vector(m_openList, neighbour))
			{
				// see if the potential cost is lower than the cost of the node
				if (currentNode->m_gCost > neighbour->m_gCost)
				{
					continue;
				}
			}

			m_openList.push_back(neighbour);
		}
	}
}

void Ghost::Move(std::vector<std::vector<Tile>>& tiles)
{
	m_updateTicks++;
	if (m_updateTicks == 10 || m_path.empty())
	{
		UpdatePathFinding(tiles);
		m_updateTicks = 0;
	}

	// Pop off the front of the path

	if (!m_path.empty())
	{
		Tile* destination = m_path.top();
		m_path.pop();

		m_position = destination->m_position;
	}
}

void Ghost::UpdatePathFinding(std::vector<std::vector<Tile>>& tiles)
{
	// Find the directions that the ghost CAN'T Travel in
	CheckForBlockades(tiles);

	switch (m_state)
	{
	case eGhostState::e_House:
		// TODO: Move up and down and wait to be released
		break;
	case eGhostState::e_Chase:
		ChaseModePathFinding(tiles);
		break;
	case eGhostState::e_Scatter:
		ScatterModePathFinding(tiles);
		break;
	case eGhostState::e_Frightened:
		// TODO: Run away from PacMan, move wherever is furthest away
		break;
	default:;
	}
}

void Ghost::ChaseModePathFinding(std::vector<std::vector<Tile>>& tiles)
{
	switch (m_type)
	{
	case eGhostType::e_Blinky:
		// Chase pacman and try to get behind him
	{
		const eDirection pacManDirection = m_pacMan.GetDirection();

		const sf::Vector2i pacManPosition = m_pacMan.GetPosition();

		// Grab the First legal tile that is behind pacman
		switch (pacManDirection)
		{
		case eDirection::e_None:
			AStarPathFinding(tiles, m_position, pacManPosition);
			break;
		case eDirection::e_Up:
			// If the tile directly below is legal, move towards that

			if (tiles[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
				[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
			{
				const sf::Vector2i tileBelowPosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

				AStarPathFinding(tiles, m_position, tileBelowPosition);
			} else
			{
				AStarPathFinding(tiles, m_position, pacManPosition);
			}
			break;
		case eDirection::e_Down:
			// If the tile directly above is legal, move towards it
			if (tiles[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
				[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
			{
				const sf::Vector2i tileAbovePosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

				AStarPathFinding(tiles, m_position, tileAbovePosition);
			} else
			{
				AStarPathFinding(tiles, m_position, pacManPosition);
			}
			break;
		case eDirection::e_Left:
			// If the tile directly to the left is legal, move towards it
			if (tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
				[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_type == eTileType::e_Path)
			{
				const sf::Vector2i leftTilePosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_position;

				AStarPathFinding(tiles, m_position, leftTilePosition);
			} else
			{
				AStarPathFinding(tiles, m_position, pacManPosition);
			}
			break;
		case eDirection::e_Right:
			// If the tile directly to the right is legal, move towards it
			if (tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
				[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_type == eTileType::e_Path)
			{
				const sf::Vector2i rightTilePosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_position;

				AStarPathFinding(tiles, m_position, rightTilePosition);
			} else
			{
				AStarPathFinding(tiles, m_position, pacManPosition);
			}break;
		default:
			break;
		}
	}
	break;


	case eGhostType::e_Pinky:
	{
		// Chase pacman and try to get in front of him
		{
			const eDirection pacManDirection = m_pacMan.GetDirection();

			const sf::Vector2i pacManPosition = m_pacMan.GetPosition();

			// Grab the First legal tile that is behind pacman
			switch (pacManDirection)
			{
			case eDirection::e_None:
				AStarPathFinding(tiles, m_position, pacManPosition);
				break;
			case eDirection::e_Down:
				// If the tile directly below is legal, move towards that

				if (tiles[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
				{
					const sf::Vector2i tileBelowPosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
						[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

					AStarPathFinding(tiles, m_position, tileBelowPosition);
				} else
				{
					AStarPathFinding(tiles, m_position, pacManPosition);
				}
				break;
			case eDirection::e_Up:
				// If the tile directly above is legal, move towards it
				if (tiles[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
				{
					const sf::Vector2i tileAbovePosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
						[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

					AStarPathFinding(tiles, m_position, tileAbovePosition);
				} else
				{
					AStarPathFinding(tiles, m_position, pacManPosition);
				}
				break;
			case eDirection::e_Right:
				// If the tile directly to the left is legal, move towards it
				if (tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_type == eTileType::e_Path)
				{
					const sf::Vector2i leftTilePosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
						[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_position;

					AStarPathFinding(tiles, m_position, leftTilePosition);
				} else
				{
					AStarPathFinding(tiles, m_position, pacManPosition);
				}
				break;
			case eDirection::e_Left:
				// If the tile directly to the right is legal, move towards it
				if (tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_type == eTileType::e_Path)
				{
					const sf::Vector2i rightTilePosition = tiles[helpers::world_coord_to_array_index(pacManPosition.y)]
						[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_position;

					AStarPathFinding(tiles, m_position, rightTilePosition);
				} else
				{
					AStarPathFinding(tiles, m_position, pacManPosition);
				}break;
			default:
				break;
			}
		}
	}
	break;
	case eGhostType::e_Inky:
		// Patrol an area
		if (m_path.empty())
		{
			m_currentCorner++;

			if (m_currentCorner > 3)
			{
				m_currentCorner = 0;
				//std::cout << "Resetting to the top left: " << std::endl;
			}

			//std::cout << "Moving toward X : " << m_cornerPositions[m_currentCorner].x << " Y : " << m_cornerPositions[m_currentCorner].y << std::endl;

			AStarPathFinding(tiles, m_position, m_cornerPositions[m_currentCorner]);
		}

		break;
	case eGhostType::e_Clyde:
		// Move to a random position
		if (m_path.empty())
		{
			Tile& randomTile = tiles[1][1];
			do
			{
				const sf::Vector2i random(helpers::rand_range(25, 750), helpers::rand_range(50, 725));
				randomTile = tiles[helpers::world_coord_to_array_index(random.y)][helpers::world_coord_to_array_index(random.y)];
			} while (randomTile.m_type != eTileType::e_Path);
			AStarPathFinding(tiles, m_position, randomTile.m_position);
		}

		break;
	default:;
	}
}

void Ghost::ScatterModePathFinding(std::vector<std::vector<Tile>>& tiles)
{
	AStarPathFinding(tiles, m_position, m_cornerPositions[static_cast<int>(m_type)]);
}
