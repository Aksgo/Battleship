#pragma once
#include <vector>
#include "Field.h"
#include "Ship.h"
namespace ShipFunc{
	extern std::vector<Ship> createShips();
	extern void setGameShips(Field &player);
}
