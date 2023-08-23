// roguelike.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>

const int MapSize = 10;

typedef enum ERoomType
{
	Empty = 0, // 10%
	Item, // 5%
	Hostile, // 35%
	//Shop, // 1 per floor
	Wall, // 50%
	AmountOfRooms


}ERoomType;

void displayMap(const ERoomType* map)
{
	for (size_t x = 0; x < MapSize; x++)
	{
		for (size_t y = 0; y < MapSize; y++)
		{
			std::cout << (*map);
			map++;
		}
		std::cout << std::endl;
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

//(x * mapsize) + y


int main()
{
	const int mapOverallSize = MapSize * MapSize;
	srand(0);
	ERoomType map[mapOverallSize] = { Empty };
	addRandomRooms(map, mapOverallSize * 0.1f, ERoomType::Wall);
	displayMap(map);


	int wallAmount = mapOverallSize * 0.4f;
	int itemAmount = mapOverallSize * 0.05f;
	int hostileAmount = mapOverallSize * 0.35f;

	int wallUnits = wallAmount / wallAmount;

	bool xory = (rand() % 2);

	int position = (rand() % MapSize);

	int otherpos = 0;

	//x
	//if (xory)
	{
		bool up = (rand() % 2);

		if (up)
		{
			otherpos = position;
		}
		else
		{
			otherpos = (mapOverallSize - MapSize) + position;
		}
	}
	//y
	//else
	{
		int leftorright = ((MapSize - 1) * position) + (rand() % 2);

			otherpos = leftorright;
	}

	//std::cout << endy;
}