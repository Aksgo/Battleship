/**
 * Unit Tests for Grid Class
 */

#include "../include/game.h"
#include <cassert>
#include <iostream>

void testGridCreation() {
    std::cout << "Testing Grid Creation... ";
    Grid grid(10);
    assert(grid.getSize() == 10);
    std::cout << "PASSED\n";
}

void testGridBoundaries() {
    std::cout << "Testing Grid Boundaries... ";
    Grid grid(8);
    
    // Test valid positions
    assert(grid.isValidPosition({0, 0}));
    assert(grid.isValidPosition({7, 7}));
    
    // Test invalid positions
    assert(!grid.isValidPosition({-1, 0}));
    assert(!grid.isValidPosition({0, 8}));
    assert(!grid.isValidPosition({8, 8}));
    
    std::cout << "PASSED\n";
}

void testShipPlacement() {
    std::cout << "Testing Ship Placement... ";
    Grid grid(10);
    Ship ship(ShipType::DESTROYER, 3);
    
    // Test valid placement
    assert(grid.canPlaceShip(ship, {2, 3}, true)); // Horizontal
    assert(grid.placeShip(ship, {2, 3}, true));
    
    // Test invalid placement (out of bounds)
    assert(!grid.canPlaceShip(ship, {9, 9}, true));
    
    // Test overlapping ships
    Ship anotherShip(ShipType::PATROL, 2);
    assert(!grid.canPlaceShip(anotherShip, {2, 3}, true)); // Overlap
    
    std::cout << "PASSED\n";
}

void testAttackLogic() {
    std::cout << "Testing Attack Logic... ";
    Grid grid(10);
    Ship ship(ShipType::BATTLESHIP, 4);
    grid.placeShip(ship, {0, 0}, true);
    
    // Test hit
    assert(grid.receiveAttack({0, 0}) == AttackResult::HIT);
    assert(grid.getCell({0, 0}) == CellState::HIT);
    
    // Test miss
    assert(grid.receiveAttack({5, 5}) == AttackResult::MISS);
    assert(grid.getCell({5, 5}) == CellState::MISS);
    
    // Test already attacked
    assert(grid.receiveAttack({0, 0}) == AttackResult::ALREADY_ATTACKED);
    
    std::cout << "PASSED\n";
}

void testShipSinking() {
    std::cout << "Testing Ship Sinking... ";
    Grid grid(10);
    Ship ship(ShipType::PATROL, 2);
    grid.placeShip(ship, {0, 0}, true);
    
    // Hit first segment
    grid.receiveAttack({0, 0});
    assert(!ship.isSunk());
    
    // Hit second segment - ship should sink
    grid.receiveAttack({0, 1});
    assert(ship.isSunk());
    
    std::cout << "PASSED\n";
}

void testGridDisplay() {
    std::cout << "Testing Grid Display... ";
    Grid grid(5);
    
    // Test that display doesn't crash
    grid.display(false); // Without ships
    grid.display(true);  // With ships
    
    std::cout << "PASSED\n";
}

int main() {
    std::cout << "Running Grid Tests...\n\n";
    
    testGridCreation();
    testGridBoundaries();
    testShipPlacement();
    testAttackLogic();
    testShipSinking();
    testGridDisplay();
    
    std::cout << "\nAll Grid tests passed! ✅\n";
    return 0;
}