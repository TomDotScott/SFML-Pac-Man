#include "Ghost.h"

#include <iostream>

#include "Constants.h"
#include "Helpers.h"

Ghost::Ghost(const eGhostType type, std::vector<std::vector<Tile>>& grid, PacMan& pacMan) :
	Entity(sf::Vector2i(),
		constants::k_gridCellSize,
		eDirection::e_None,
		sf::Color::White),
	m_pacMan(pacMan),
	m_type(type),
	m_state(eGhostState::e_Chase),
	m_homeTimer(0.f),
	m_updateTicks(0),
	m_grid(grid),
	m_currentCorner(0)
{
	// define the corners of the map for patrolling
	// and scattering

	switch (m_type)
	{
	case eGhostType::e_Blinky:
		m_colour = sf::Color::Red;
		break;
	case eGhostType::e_Pinky:
		m_colour = sf::Color::Magenta;
		break;
	case eGhostType::e_Inky:
		m_colour = sf::Color::Cyan;
		break;
	case eGhostType::e_Clyde:
		m_colour = sf::Color(255, 165, 0);
		break;
	default:;
	}

	m_position = constants::k_cornerPositions[static_cast<int>(m_type)];
}

void Ghost::Update()
{
	if (m_state == eGhostState::e_Frightened && m_position == constants::k_homePositions[static_cast<int>(m_type)])
	{
		m_homeTimer += m_clock.getElapsedTime().asSeconds();
		if (m_homeTimer >= constants::k_ghostHomeTime)
		{
			m_state = eGhostState::e_Chase;
			m_homeTimer = 0.f;
		}
	} else
	{
		Move();
		CheckPacManCollisions();
	}

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

	if (m_state != eGhostState::e_Frightened)m_shape.setFillColor(m_colour);
	else
	{
		const sf::Color frightenedColour = { 0, 19, 142 };
		if (m_position == constants::k_homePositions[static_cast<int>(m_type)])
		{
			// Blend from blue to the normal ghost colour
			const float normalisedTimer = m_homeTimer / static_cast<float>(constants::k_ghostHomeTime);


			const sf::Uint32 lerpedColour = helpers::interpolate(frightenedColour.toInteger(), m_colour.toInteger(), normalisedTimer);

			m_shape.setFillColor(sf::Color(lerpedColour));
		} else m_shape.setFillColor(frightenedColour);
	}

	m_shape.setPosition(static_cast<sf::Vector2f>(m_position));

	window.draw(m_shape);
}

void Ghost::Reset()
{
	m_state = eGhostState::e_Chase;
	m_position = constants::k_cornerPositions[static_cast<int>(m_type)];

	// Empty the path if it exists
	while (!m_path.empty())
	{
		m_path.pop();
	}
}

const eGhostState& Ghost::GetGhostState() const
{
	return m_state;
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

std::vector<Tile*> Ghost::GetNeighbourNodes(Tile* currentNode) const
{
	std::vector<Tile*> neighbours;

	const int xIndex = helpers::world_coord_to_array_index(currentNode->m_position.x);
	const int yIndex = helpers::world_coord_to_array_index(currentNode->m_position.y);

	// Find the 4 neighbour positions if they are valid
	if (xIndex - 1 >= 0)
	{
		// Left
		neighbours.push_back(&m_grid[yIndex][xIndex - 1]);
	}

	if (xIndex + 1 < static_cast<int>(m_grid[0].size()))
	{
		// Right
		neighbours.push_back(&m_grid[yIndex][xIndex + 1]);
	}

	// Top
	if (yIndex - 1 >= 0)
	{
		neighbours.push_back(&m_grid[yIndex - 1][xIndex]);
	}

	// Bottom
	if (yIndex + 1 < static_cast<int>(m_grid.size()))
	{
		neighbours.push_back(&m_grid[yIndex + 1][xIndex]);
	}

	return neighbours;
}

void Ghost::AStarPathFinding(const sf::Vector2i startPosition, const sf::Vector2i endPosition)
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

	Tile* startNode = &m_grid[startNodeIndices.y][startNodeIndices.x];
	Tile* endNode = &m_grid[endNodeIndices.y][endNodeIndices.x];

	m_openList.clear();
	m_closedList.clear();

	// Cycle through grid, set g to infinite and calculate f cost
	for (auto& tile : m_grid)
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


		for (auto& neighbour : GetNeighbourNodes(currentNode))
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

void Ghost::Move()
{
	m_updateTicks++;
	if (m_updateTicks == 10 || m_path.empty())
	{
		UpdatePathFinding();
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

void Ghost::CheckPacManCollisions()
{
	if (m_state != eGhostState::e_Frightened)
	{
		if (m_position == m_pacMan.GetPosition())
		{
			if (m_pacMan.GetPacManState() == ePacManState::e_PowerUp)
			{
				m_state = eGhostState::e_Frightened;
				m_pacMan.AddPoints(1000);
			} else
			{
				m_pacMan.SetIsAlive(false);
			}
			std::cout << "I hit PacMan!" << std::endl;
		}
	}
}

void Ghost::UpdatePathFinding()
{
	switch (m_state)
	{
	case eGhostState::e_Chase:
		ChaseModePathFinding();
		break;
	case eGhostState::e_Scatter:
		AStarPathFinding(m_position, constants::k_cornerPositions[static_cast<int>(m_type)]);
		break;
	case eGhostState::e_Frightened:
		AStarPathFinding(m_position, constants::k_homePositions[static_cast<int>(m_type)]);
		break;
	default:;
	}
}

void Ghost::ChaseModePathFinding()
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
			AStarPathFinding(m_position, pacManPosition);
			break;
		case eDirection::e_Up:
			// If the tile directly below is legal, move towards that

			if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
				[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
			{
				const sf::Vector2i tileBelowPosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

				AStarPathFinding(m_position, tileBelowPosition);
			} else
			{
				AStarPathFinding(m_position, pacManPosition);
			}
			break;
		case eDirection::e_Down:
			// If the tile directly above is legal, move towards it
			if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
				[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
			{
				const sf::Vector2i tileAbovePosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

				AStarPathFinding(m_position, tileAbovePosition);
			} else
			{
				AStarPathFinding(m_position, pacManPosition);
			}
			break;
		case eDirection::e_Left:
			// If the tile directly to the left is legal, move towards it
			if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
				[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_type == eTileType::e_Path)
			{
				const sf::Vector2i leftTilePosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_position;

				AStarPathFinding(m_position, leftTilePosition);
			} else
			{
				AStarPathFinding(m_position, pacManPosition);
			}
			break;
		case eDirection::e_Right:
			// If the tile directly to the right is legal, move towards it
			if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
				[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_type == eTileType::e_Path)
			{
				const sf::Vector2i rightTilePosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_position;

				AStarPathFinding(m_position, rightTilePosition);
			} else
			{
				AStarPathFinding(m_position, pacManPosition);
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
				AStarPathFinding(m_position, pacManPosition);
				break;
			case eDirection::e_Down:
				// If the tile directly below is legal, move towards that

				if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
				{
					const sf::Vector2i tileBelowPosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y) + 1]
						[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

					AStarPathFinding(m_position, tileBelowPosition);
				} else
				{
					AStarPathFinding(m_position, pacManPosition);
				}
				break;
			case eDirection::e_Up:
				// If the tile directly above is legal, move towards it
				if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
					[helpers::world_coord_to_array_index(pacManPosition.x)].m_type == eTileType::e_Path)
				{
					const sf::Vector2i tileAbovePosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y) - 1]
						[helpers::world_coord_to_array_index(pacManPosition.x)].m_position;

					AStarPathFinding(m_position, tileAbovePosition);
				} else
				{
					AStarPathFinding(m_position, pacManPosition);
				}
				break;
			case eDirection::e_Right:
				// If the tile directly to the left is legal, move towards it
				if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_type == eTileType::e_Path)
				{
					const sf::Vector2i leftTilePosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
						[helpers::world_coord_to_array_index(pacManPosition.x) - 1].m_position;

					AStarPathFinding(m_position, leftTilePosition);
				} else
				{
					AStarPathFinding(m_position, pacManPosition);
				}
				break;
			case eDirection::e_Left:
				// If the tile directly to the right is legal, move towards it
				if (m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
					[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_type == eTileType::e_Path)
				{
					const sf::Vector2i rightTilePosition = m_grid[helpers::world_coord_to_array_index(pacManPosition.y)]
						[helpers::world_coord_to_array_index(pacManPosition.x) + 1].m_position;

					AStarPathFinding(m_position, rightTilePosition);
				} else
				{
					AStarPathFinding(m_position, pacManPosition);
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

			if (m_currentCorner > 3) m_currentCorner = 0;

			AStarPathFinding(m_position, constants::k_cornerPositions[m_currentCorner]);
		}

		break;
	case eGhostType::e_Clyde:
		// Move to a random position
		if (m_path.empty())
		{
			Tile& randomTile = m_grid[1][1];
			do
			{
				const sf::Vector2i random(helpers::rand_range(25, 750), helpers::rand_range(50, 725));
				randomTile = m_grid[helpers::world_coord_to_array_index(random.y)][helpers::world_coord_to_array_index(random.y)];
			} while (randomTile.m_type != eTileType::e_Path);
			AStarPathFinding(m_position, randomTile.m_position);
		}

		break;
	default:;
	}
}