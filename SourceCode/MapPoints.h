#include "EDirection.h"
#include <deque>
#include <set>
#include <algorithm>

static int GetEndPosition(bool edge, int pos, int MapSize, bool xPos)
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

static int GetNextWallPosition(int MapSize, EDirection direction, int position, int mapOverallSize)
{

	int newPositon = -1;

	if (position >= mapOverallSize || position < 0)
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

	return (newPositon < mapOverallSize && newPositon >= 0) ? newPositon : -1;
}


static int GetNextWallPosition(int MapSize, EDirection direction, int position)
{
	return GetNextWallPosition(MapSize, direction, position, MapSize * MapSize);
}

static int GetRandomNumber(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

static std::set<int> GetAvailbleNeighbourCells(int position, int MapSize)
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

static int getElementByIndex(const std::set<int>& mySet, size_t index) {
	if (index < mySet.size()) {
		auto it = mySet.begin();
		std::advance(it, index);
		return *it;
	}
	else {
		throw std::out_of_range("Index out of range");
	}
}

static std::vector<ERoomType> MazeGeneration(int MapSize, int startPoint)
{
	std::vector<ERoomType> map(MapSize * MapSize, ERoomType::Empty);
	int nextPosition = startPoint;

	std::set<int> otherAvailbleCells;

	std::set<int> availbleCells;

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

		nextPosition = getElementByIndex(availbleCells, GetRandomNumber(0, availbleCells.size() - 1));
		map[nextPosition] = ERoomType::Path;
		availbleCells.erase(nextPosition);

		otherAvailbleCells.insert(availbleCells.begin(), availbleCells.end());
		availbleCells.clear();
	}

	return map;
}

class Cell
{
public:

	Cell(int _f, int _h, int _g, int _pos) { f = _f; h = _h; g = _g; position = _pos; };
	Cell* parent;
	int f;
	int h;
	int g = 1;
	int position;
};

static int FindDistance(int pos1, int pos2, int mapsize)
{
	int dis = floor(abs(pos1 - pos2) / mapsize);
	int remainder = abs(pos1 - pos2) % mapsize;


	return remainder + dis;
}

static void SortList(std::deque<Cell*>& list)
{
	std::sort(list.begin(), list.end(), [](const Cell* a, const Cell* b)
		{
			return a->f < b->f;
		});
}

static void SortClosedList(std::deque<Cell*>& list)
{
	std::sort(list.begin(), list.end(), [](const Cell* a, const Cell* b)
		{
			return a->h < b->h;
		});
}


static Cell* AStar(const std::vector<ERoomType>& map, int start, int end, int MapSize, ERoomType room = ERoomType::Wall)
{
	std::deque<Cell*> closedList;
	std::deque<Cell*> openList;

	const int startDistance = FindDistance(start, end, MapSize);
	Cell* startCell = new Cell(startDistance, startDistance, 0, start);

	openList.push_back(startCell);

	while (!openList.empty())
	{
		SortList(openList);
		auto q = openList.front();
		openList.pop_front();
		std::set<int> negihbours;
		bool found = false;

		for (const auto cell : GetAvailbleNeighbourCells(q->position, MapSize))
		{
			if (map[cell] < room)
			{
				negihbours.insert(cell);
			}
		}

		bool skip = false;
		for (const auto cell : negihbours)
		{
			skip = false;
			const int distance = FindDistance(cell, end, MapSize);
			const int parentCost = q->g;
			Cell* newCell = new Cell(distance + parentCost, distance, parentCost + 1, cell);
			newCell->parent = q;

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

		if (!skip)
		{
			closedList.push_back(q);
		}


		if (found)
		{
			break;
		}
	}

	if (openList.empty())
	{
		SortClosedList(closedList);

		openList.push_back(closedList.front());
	}

	return openList.back();
}