
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "MapPoints.h"
#include "EDirection.h"

TEST_CASE("testing Y pos ") {
    // Y
    //Left
    CHECK(GetEndPosition(false, 0, 5, false) == 0);
    CHECK(GetEndPosition(false, 1, 5, false) == 5);
    CHECK(GetEndPosition(false, 2, 5, false) == 10);
    CHECK(GetEndPosition(false, 3, 5, false) == 15);
    CHECK(GetEndPosition(false, 4, 5, false) == 20);
    CHECK(GetEndPosition(false, 5, 5, false) == -1);
    CHECK(GetEndPosition(false, 53, 5, false) == -1);
    CHECK(GetEndPosition(false, 123, 5, false) == -1);

   //Right
    CHECK(GetEndPosition(true, 0, 5, false) == 4);
    CHECK(GetEndPosition(true, 1, 5, false) == 9);
    CHECK(GetEndPosition(true, 2, 5, false) == 14);
    CHECK(GetEndPosition(true, 3, 5, false) == 19);
    CHECK(GetEndPosition(true, 4, 5, false) == 24);

    // X
    //Left
    CHECK(GetEndPosition(false, 0, 5, true) == 0);
    CHECK(GetEndPosition(false, 1, 5, true) == 1);
    CHECK(GetEndPosition(false, 2, 5, true) == 2);
    CHECK(GetEndPosition(false, 3, 5, true) == 3);
    CHECK(GetEndPosition(false, 4, 5, true) == 4);
          
    //Righ
    CHECK(GetEndPosition(true, 0, 5, true) == 20);
    CHECK(GetEndPosition(true, 1, 5, true) == 21);
    CHECK(GetEndPosition(true, 2, 5, true) == 22);
    CHECK(GetEndPosition(true, 3, 5, true) == 23);
    CHECK(GetEndPosition(true, 4, 5, true) == 24);
}

TEST_CASE("Testing Neighbour Cells") {

    const int mapSize = 330;

    CHECK(GetNextWallPosition(mapSize, EDirection::East, mapSize * 2 - 1) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::East, mapSize * 3 - 1) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::East, mapSize * 3 - 2) == mapSize * 3 - 1);
    CHECK(GetNextWallPosition(mapSize, EDirection::East, mapSize * mapSize - 1) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::East, mapSize * (mapSize + 1)) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::East, -3) == -1);

    CHECK(GetNextWallPosition(mapSize, EDirection::West, mapSize * 2 - 1) == mapSize * 2 - 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::West, mapSize) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::West, 0) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::West, mapSize * 2 + 1) == mapSize * 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::West, mapSize * (mapSize + 1)) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::West, -3) == -1);

    CHECK(GetNextWallPosition(mapSize, EDirection::North, 2) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::North, mapSize + 2) == 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::North, mapSize) == 0);
    CHECK(GetNextWallPosition(mapSize, EDirection::North, mapSize * 4 + 2) == mapSize * 3 + 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::North, mapSize * (mapSize + 1)) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::North, -3) == -1);

    CHECK(GetNextWallPosition(mapSize, EDirection::South, 2) == mapSize + 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::South, mapSize + 2) == mapSize * 2 + 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::South, mapSize) == mapSize * 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::South, mapSize * 3 + 2) == mapSize * 4 + 2);
    CHECK(GetNextWallPosition(mapSize, EDirection::South, mapSize * (mapSize + 1)) == -1);
    CHECK(GetNextWallPosition(mapSize, EDirection::South, -3) == -1);
}

TEST_CASE("Testing random number function") {

    int min = -10;
    int max = 10;
    for (int i = 0; i < 100; i++)
    {
        auto test = GetRandomNumber(min, max);
        //std::cout << test << std::endl;
        CHECK(test < max + 1);
        CHECK(test > min - 1);
    }
}