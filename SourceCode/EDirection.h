#pragma once
enum EDirection
{
	North = 0,
	East = 1,
	South = 2,
	West = 3,
	Count
};

EDirection& operator++(EDirection& en) 
{ 
	if (en + 1 >= Count)
	{
		en = North;
		return en;
	}

	en = static_cast<EDirection>(en + 1); 
	return en; 
 }

EDirection operator++(EDirection& e, int) {
	EDirection result = e;
	++e;  // Use the prefix ++ operator to increment
	return result;
}

enum ERoomType
{
	Empty = 0, // 10%
	Path,
	optimal,
	Item, // 5%
	Hostile, // 35%
	//Shop, // 1 per floor
	Wall, // 50%
	Start,
	End,
	AmountOfRooms
};
