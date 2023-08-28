#include "EDirection.h"
#include <deque>
#include <set>
#include <algorithm>

static const int GetEndPosition(bool edge, int pos, int MapSize, bool xPos)
{
	if (pos >= MapSize)
	{
		return -1;
	}

	if (xPos)
	{
		if (!edge)
		{
			return pos;
		}

		const int mapOverallSize = MapSize * MapSize;

		return (mapOverallSize - MapSize) + pos;
	}

	return (MapSize * pos) + (edge * (MapSize - 1));
}

static const int GetNextWallPosition(int MapSize, EDirection direction, int position)
{
	const int mapOverallSize = MapSize * MapSize;

	int newPositon = -1;

	if (position > mapOverallSize || position < 0)
	{
		return -1;
	}

	if (direction == EDirection::North)
	{
		newPositon = position - MapSize;
	}
	else if (direction == EDirection::East)
	{
		if ((position + 1) % MapSize)
		{
			newPositon = position + 1;
		}
	}
	else if (direction == EDirection::West)
	{
		if ((position) % MapSize)
		{
			newPositon = position - 1;
		}
	}
	else
	{
		newPositon = position + MapSize;
	}

	return (newPositon <= mapOverallSize && newPositon >= 0) ? newPositon : -1;
}

static const int GetRandomNumber(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

const std::set<int> GetAvailbleNeighbourCells(int position, int MapSize)
{
	std::set<int> cells;

	for (int i = EDirection::North; i < (int)EDirection::Count; i++)
	{
		int cell = GetNextWallPosition(MapSize, (EDirection)i, position);

		if (cell != -1)
		{
			cells.insert(cell);
		}
	}
	return cells;
}

int getElementByIndex(const std::set<int>& mySet, size_t index) {
	if (index < mySet.size()) {
		auto it = mySet.begin();
		std::advance(it, index);
		return *it;
	}
	else {
		throw std::out_of_range("Index out of range");
	}
}

void MazeGeneration(ERoomType* map, int MapSize, int startPoint)
{
	int nextPosition = startPoint;

	std::set<int> otherAvailbleCells;

	std::set<int> availbleCells = GetAvailbleNeighbourCells(nextPosition, MapSize);

	while (true)
	{
		for (const auto cell : GetAvailbleNeighbourCells(nextPosition, MapSize))
		{
			if (map[cell] == ERoomType::Empty)
			{
				availbleCells.insert(cell);
			}
		}

		if (availbleCells.empty() && !otherAvailbleCells.empty())
		{
			while (!otherAvailbleCells.empty())
			{
				int cell = *otherAvailbleCells.begin();
				if (map[cell] != ERoomType::Empty)
				{
					otherAvailbleCells.erase(cell);
				}
				else
				{

					availbleCells.insert(cell);
					otherAvailbleCells.erase(cell);
					break;
				}
			}


		}
		
		if (availbleCells.empty() && otherAvailbleCells.empty())
		{
			break;
		}

		if (availbleCells.size() == 2)
		{
			const int wallPosition = getElementByIndex(availbleCells, GetRandomNumber(0, availbleCells.size() - 1));
			map[wallPosition] = ERoomType::Wall;
			availbleCells.erase(wallPosition);
		}

		//if (availbleCells.size() > 2 && !otherAvailbleCells.empty())
		//{
		//	int wallPosition = getElementByIndex(availbleCells, GetRandomNumber(0, availbleCells.size() - 1));
		//	map[wallPosition] = ERoomType::Wall;
		//	availbleCells.erase(wallPosition);

		//	wallPosition = getElementByIndex(availbleCells, GetRandomNumber(0, availbleCells.size() - 1));
		//	map[wallPosition] = ERoomType::Wall;
		//	availbleCells.erase(wallPosition);
		//}

		nextPosition = getElementByIndex(availbleCells, GetRandomNumber(0, availbleCells.size() - 1));
		map[nextPosition] = ERoomType::Path;
		availbleCells.erase(nextPosition);

		otherAvailbleCells.insert(availbleCells.begin(), availbleCells.end());
		availbleCells.clear();
	}
}

class Cell
{
public:
	Cell* parent;
	int f;
	int g = 1;
	int position;
};

int FindDistance(int pos1, int pos2)
{
	return abs(pos1 - pos2);
}

int FindF(int h)
{
	return h + 1;
}

//bool find(std::deque<Cell*> q)
//{
//	std::deque<Cell*>::iterator itr;
//	itr = find(q.begin(), q.end(), 2);
//	if (itr != q.end())
//	{
//		return true;
//	}
//
//	return false;
//}


Cell* AStar(ERoomType* map, int start, int end, int MapSize)
{
	std::deque<Cell*> closedList;
	std::deque<Cell*> openList;

	Cell* startCell = new Cell();

	startCell->position = start;
	startCell->f = FindDistance(startCell->position, end);
	startCell->g = 0;

	openList.push_back(startCell);

	while (!openList.empty())
	{
		std::sort(openList.begin(), openList.end(), [](const Cell* a, const Cell* b) {
			return a->f < b->f;
			});
		auto q = openList.front();
		openList.pop_front();
		std::set<int> negihbours;
		bool found = false;

		for (const auto cell : GetAvailbleNeighbourCells(q->position, MapSize))
		{
			if (map[cell] == ERoomType::Path || map[cell] == ERoomType::End || map[cell] == ERoomType::Empty)
			{
				negihbours.insert(cell);
			}
		}

		for (const auto cell : negihbours)
		{
			bool skip = false;

			Cell* newCell = new Cell();
			newCell->parent = q;
			newCell->position = cell;
			newCell->g = q->g + 1;
			newCell->f = newCell->g + FindDistance(newCell->position, end);

			for (auto it = openList.begin(); it != openList.end(); ++it)
			{
				if ((*it)->position == newCell->position && (*it)->f <= newCell->f)
				{
					skip = true;
					break;
				}
			}

			for (auto it = closedList.begin(); it != closedList.end(); ++it)
			{
				if ((*it)->position == newCell->position && (*it)->f <= newCell->f)
				{
					skip = true;
					break;
				}
			}

			if (!skip && newCell->f < MapSize * MapSize)
			{
				openList.push_back(newCell);
			}

			if (cell == end)
			{
				found = true;
				break;
			}
		}

		closedList.push_back(q);

		if (found)
		{
			break;
		}


	}
	return openList.back();
}