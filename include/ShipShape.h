#include <utility>
#include <string>
#include <vector>

// L3 shape for 2D support
struct ShipShape{
	std::string name;
	std::vector<std::pair<int, int>> offsets; 
};
extern const ShipShape L3Shape;

