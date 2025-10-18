/**
 * Enhanced AI System for Battleship Game
 * Features: Multiple difficulty levels, smart targeting, adaptive strategies
 */

#pragma once
#include "game.h"
#include <vector>
#include <queue>
#include <random>

namespace BattleshipAI {

enum class Difficulty {
    EASY,      // Random shooting
    MEDIUM,    // Basic hunt/target strategy
    HARD,      // Probability-based targeting
    EXPERT     // Pattern recognition + learning
};

enum class AIState {
    HUNTING,   // Searching for ships
    TARGETING  // Found a ship, destroying it
};

class EnhancedAI {
private:
    Difficulty difficulty;
    AIState currentState;
    
    // Hunting phase
    std::vector<Position> possibleTargets;
    std::vector<Position> hitPositions;
    std::vector<Position> missPositions;
    
    // Targeting phase
    std::queue<Position> targetQueue;
    Position lastHit;
    
    // Probability tracking
    std::vector<std::vector<int>> probabilityGrid;
    
    // Random generator
    std::mt19937 rng;

    void initializeProbabilityGrid();
    void updateProbabilityGrid();
    Position getBestProbabilityTarget();
    void generateTargetingShots(const Position& hit);
    bool isValidTarget(const Position& pos) const;

public:
    EnhancedAI(Difficulty diff = Difficulty::MEDIUM);
    
    void setDifficulty(Difficulty diff);
    Difficulty getDifficulty() const;
    
    Position makeMove(const Grid& enemyGrid);
    void recordResult(const Position& pos, bool wasHit, bool wasSunk);
    
    void reset();
    
    // Strategy methods
    Position makeEasyMove();
    Position makeMediumMove();
    Position makeHardMove();
    Position makeExpertMove();
};

// Utility functions
std::string difficultyToString(Difficulty diff);
Difficulty stringToDifficulty(const std::string& str);

} // namespace BattleshipAI