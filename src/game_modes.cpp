#include "game_modes.h"
#include <iostream>
#include <algorithm>

namespace BattleshipModes {

// Base GameMode implementation
GameMode::GameMode(const std::string& name, const std::string& desc) 
    : name(name), description(desc) {}

std::string GameMode::getName() const { return name; }
std::string GameMode::getDescription() const { return description; }

// Salvo Mode Implementation
SalvoMode::SalvoMode(int shots) 
    : GameMode("Salvo", "Fire multiple shots per turn based on remaining ships"), 
      baseShots(shots) {}

void SalvoMode::initialize(Game& game) {
    std::cout << "Salvo Mode: Each player fires shots equal to remaining ships!\n";
}

void SalvoMode::onTurnStart(Player& player) {
    int shots = getShotsForPlayer(player);
    std::cout << "You have " << shots << " shots this turn.\n";
}

void SalvoMode::onTurnEnd(Player& player) {
    // Nothing special needed at turn end
}

bool SalvoMode::checkWinCondition(const Game& game) {
    // Standard win condition - all enemy ships sunk
    return game.isGameOver();
}

int SalvoMode::getShotsForPlayer(const Player& player) const {
    // Number of shots equals number of remaining ships
    int remainingShips = 0;
    for (const auto& ship : player.getShips()) {
        if (!ship.isSunk()) {
            remainingShips++;
        }
    }
    return std::max(1, remainingShips); // Always at least 1 shot
}

// Mine Warfare Mode Implementation  
MineWarfareMode::MineWarfareMode(int mines)
    : GameMode("Mine Warfare", "Strategic mines are placed on the battlefield"),
      mineCount(mines) {}

void MineWarfareMode::initialize(Game& game) {
    std::cout << "Mine Warfare: " << mineCount << " mines placed on each grid!\n";
    
    // Place mines randomly (simplified - would need access to grid)
    // In real implementation, this would place mines on both player grids
}

void MineWarfareMode::onTurnStart(Player& player) {
    // Check if player hit a mine last turn
    // This would require tracking mine hits
}

void MineWarfareMode::onTurnEnd(Player& player) {
    // Mine effects would be processed here
}

bool MineWarfareMode::checkWinCondition(const Game& game) {
    return game.isGameOver();
}

bool MineWarfareMode::checkMineHit(const Position& pos) const {
    // Check if position contains a mine
    return std::find(minePositions.begin(), minePositions.end(), pos) != minePositions.end();
}

void MineWarfareMode::triggerMine(Player& player, const Position& pos) {
    std::cout << "BOOM! Mine hit at " << char('A' + pos.x) << pos.y + 1 << "!\n";
    // In real implementation, this would cause damage to surrounding area
}

// Game Mode Manager Implementation
GameModeManager::GameModeManager() : currentMode(nullptr) {
    // Register default modes
    registerMode(std::make_unique<SalvoMode>());
    registerMode(std::make_unique<MineWarfareMode>());
}

void GameModeManager::registerMode(std::unique_ptr<GameMode> mode) {
    availableModes.push_back(std::move(mode));
}

void GameModeManager::setCurrentMode(const std::string& modeName) {
    for (auto& mode : availableModes) {
        if (mode->getName() == modeName) {
            currentMode = mode.get();
            return;
        }
    }
    currentMode = nullptr; // Mode not found
}

GameMode* GameModeManager::getCurrentMode() const {
    return currentMode;
}

std::vector<std::string> GameModeManager::getAvailableModeNames() const {
    std::vector<std::string> names;
    for (const auto& mode : availableModes) {
        names.push_back(mode->getName());
    }
    return names;
}

void GameModeManager::initializeCurrentMode(Game& game) {
    if (currentMode) {
        currentMode->initialize(game);
    }
}

} // namespace BattleshipModes