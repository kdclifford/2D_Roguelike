// roguelike.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include "MapPoints.h"
#include "EDirection.h"

const int MapSize = 100;

void displayMap(const ERoomType* map)
{
	std::cout << std::endl;

	for (size_t x = 0; x < MapSize; x++)
	{
		for (size_t y = 0; y < MapSize; y++)
		{
			std::cout << (*map);
			map++;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
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

int main()
{
	const int mapOverallSize = MapSize * MapSize;
	srand(10);
	ERoomType map[mapOverallSize] = { ERoomType::Empty };
	int position = GetRandomNumber(0, MapSize - 1);

	bool edge = GetRandomNumber(0, 1);
	bool xory = GetRandomNumber(0, 1);
	int endPos = GetEndPosition(edge, position, MapSize, xory);
	map[endPos] = ERoomType::End;

	int startPos = mapOverallSize % 2 == 0 ? mapOverallSize * 0.5f + (MapSize * 0.5f) : mapOverallSize * 0.5f;

	map[startPos] = ERoomType::Start;

	MazeGeneration(map, MapSize, startPos);
	displayMap(map);

	auto endcell = AStar(map, startPos, endPos, MapSize);

	while(true)
	{
		endcell = endcell->parent;
		if (endcell == nullptr)
		{
			break;
		}

		auto position = endcell->position;
		//std::cout << endcell->position << std::endl;
		map[position] = ERoomType::optimal;

	}



	////addRandomRooms(map, mapOverallSize * 0.1f, ERoomType::Wall);

	//int wallOrigins = mapOverallSize * 0.1f;

	//for (int i = 0; i < wallOrigins; i++)
	//{
	//	position = GetRandomNumber(0, MapSize);

	//	edge = GetRandomNumber(0, 2);
	//	xory = GetRandomNumber(0, 2);
	//	int wallOriginPos = GetEndPosition(edge, position, MapSize, xory);

	//	while (map[wallOriginPos] != ERoomType::Empty)
	//	{
	//		position = GetRandomNumber(0, MapSize);

	//		edge = GetRandomNumber(0, 2);
	//		xory = GetRandomNumber(0, 2);
	//		wallOriginPos = GetEndPosition(edge, position, MapSize, xory);
	//	}

	//	map[wallOriginPos] = ERoomType::Wall;
	//}

	//int wallAmount = mapOverallSize * 0.4f;
	//int itemAmount = mapOverallSize * 0.05f;
	//int hostileAmount = mapOverallSize * 0.35f;

	//int wallUnits = wallAmount / wallAmount;



	displayMap(map);
	//std::cout << map[pos];
}