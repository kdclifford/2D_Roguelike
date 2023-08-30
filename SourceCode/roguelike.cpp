// roguelike.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include <vector>
#include "MapPoints.h"
#include "EDirection.h"

const int MapSize = 15;

void displayMap(const std::vector<ERoomType>& map)
{
	std::cout << std::endl;

	for (int i = 0; i < map.size(); i++)
	{
		if (i % MapSize == 0)
		{
			std::cout << std::endl;
		}

		std::cout << map[i];
	}
}

void addRandomRooms(ERoomType* map, int roomAmount, ERoomType roomType)
{
	ERoomType* front = map;
	int size = MapSize * MapSize;

	for (int i = 0; i < roomAmount; i++)
	{
		int randRoom = (rand() % size);
		map += randRoom;
		(*map) = roomType;
		map = front;
	}
}

std::vector<ERoomType> UpdateMap(const std::vector<ERoomType>& map, Cell* cell)
{
	std::vector<ERoomType> newMap = map;

	while (cell->parent != nullptr)
	{
		auto position = cell->position;
		//std::cout << endcell->position << std::endl;
		newMap[position] = ERoomType::OptimalPath;
		cell = cell->parent;

	}

	return newMap;
}




int main()
{
	const int mapOverallSize = MapSize * MapSize;
	srand(0);
	//ERoomType map[mapOverallSize] = { ERoomType::Empty };
	bool edge = GetRandomNumber(0, 1);
	bool xory = GetRandomNumber(0, 1);
	int position = GetRandomNumber(0, MapSize - 1);

	const int endPos = GetEndPosition(edge, position, MapSize, xory);

	const int startPos = mapOverallSize % 2 == 0 ? mapOverallSize * 0.5f + (MapSize * 0.5f) : mapOverallSize * 0.5f;
	std::vector<ERoomType> map = MazeGeneration(MapSize, startPos);

	map[startPos] = ERoomType::Start;

	
	map[endPos] = ERoomType::End;

	displayMap(map);

	auto endcell = AStar(map, startPos, endPos, MapSize);

	if (endPos != endcell->position)
	{
		map = UpdateMap(map, AStar(map, endPos, endcell->position, MapSize, ERoomType::AmountOfRooms));
	}

	map[endcell->position] = ERoomType::OptimalPath;

	map = UpdateMap(map, endcell);

	map[endPos] = ERoomType::End;

	displayMap(map);
}