#include "ai_enhanced.h"
#include <algorithm>
#include <iostream>
#include <cmath>

namespace BattleshipAI {

EnhancedAI::EnhancedAI(Difficulty diff) 
    : difficulty(diff), currentState(AIState::HUNTING), rng(std::random_device{}()) {
    initializeProbabilityGrid();
}

void EnhancedAI::initializeProbabilityGrid() {
    probabilityGrid.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    possibleTargets.clear();
    
    // Initialize all positions as possible targets
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            possibleTargets.push_back({x, y});
        }
    }
}

Position EnhancedAI::makeMove(const Grid& enemyGrid) {
    switch (difficulty) {
        case Difficulty::EASY: return makeEasyMove();
        case Difficulty::MEDIUM: return makeMediumMove();
        case Difficulty::HARD: return makeHardMove();
        case Difficulty::EXPERT: return makeExpertMove();
        default: return makeMediumMove();
    }
}

Position EnhancedAI::makeEasyMove() {
    // Simple random shooting from possible targets
    std::uniform_int_distribution<int> dist(0, possibleTargets.size() - 1);
    int index = dist(rng);
    Position pos = possibleTargets[index];
    possibleTargets.erase(possibleTargets.begin() + index);
    return pos;
}

Position EnhancedAI::makeMediumMove() {
    if (currentState == AIState::HUNTING) {
        // Hunt mode - random but avoid adjacent misses
        return getBestProbabilityTarget();
    } else {
        // Targeting mode - destroy found ship
        if (!targetQueue.empty()) {
            Position target = targetQueue.front();
            targetQueue.pop();
            
            // Remove from possible targets
            auto it = std::find(possibleTargets.begin(), possibleTargets.end(), target);
            if (it != possibleTargets.end()) {
                possibleTargets.erase(it);
            }
            return target;
        } else {
            currentState = AIState::HUNTING;
            return makeMediumMove();
        }
    }
}

Position EnhancedAI::makeHardMove() {
    updateProbabilityGrid();
    return getBestProbabilityTarget();
}

Position EnhancedAI::makeExpertMove() {
    // Expert uses probability + pattern recognition
    updateProbabilityGrid();
    Position bestTarget = getBestProbabilityTarget();
    
    // Expert avoids obvious patterns
    if (hitPositions.size() >= 2) {
        // Try to detect ship orientation and target accordingly
        // This is a simplified version - real expert AI would be more complex
    }
    
    return bestTarget;
}

Position EnhancedAI::getBestProbabilityTarget() {
    if (possibleTargets.empty()) {
        return {0, 0}; // Should not happen in valid game
    }
    
    // Find position with highest probability
    Position bestPos = possibleTargets[0];
    int maxProb = probabilityGrid[bestPos.x][bestPos.y];
    
    for (const auto& pos : possibleTargets) {
        if (probabilityGrid[pos.x][pos.y] > maxProb) {
            maxProb = probabilityGrid[pos.x][pos.y];
            bestPos = pos;
        }
    }
    
    // Remove from possible targets
    auto it = std::find(possibleTargets.begin(), possibleTargets.end(), bestPos);
    if (it != possibleTargets.end()) {
        possibleTargets.erase(it);
    }
    
    return bestPos;
}

void EnhancedAI::updateProbabilityGrid() {
    // Reset probability grid
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            probabilityGrid[x][y] = 0;
        }
    }
    
    // Calculate probabilities based on hits and ship possibilities
    for (const auto& hit : hitPositions) {
        // Increase probability around hits
        std::vector<Position> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        for (const auto& dir : directions) {
            Position newPos = {hit.x + dir.x, hit.y + dir.y};
            if (isValidTarget(newPos)) {
                probabilityGrid[newPos.x][newPos.y] += 50;
            }
        }
    }
    
    // Add basic hunting pattern (checkerboard for efficiency)
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            if ((x + y) % 2 == 0) {
                probabilityGrid[x][y] += 10;
            }
        }
    }
}

void EnhancedAI::recordResult(const Position& pos, bool wasHit, bool wasSunk) {
    if (wasHit) {
        hitPositions.push_back(pos);
        lastHit = pos;
        currentState = AIState::TARGETING;
        generateTargetingShots(pos);
        
        if (wasSunk) {
            // Clear targeting queue when ship is sunk
            targetQueue = std::queue<Position>();
            currentState = AIState::HUNTING;
            // Remove all hits of sunk ship from consideration
            // This is simplified - real implementation would track which ship was sunk
        }
    } else {
        missPositions.push_back(pos);
    }
}

void EnhancedAI::generateTargetingShots(const Position& hit) {
    // Add adjacent positions to targeting queue
    std::vector<Position> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    for (const auto& dir : directions) {
        Position newPos = {hit.x + dir.x, hit.y + dir.y};
        if (isValidTarget(newPos)) {
            targetQueue.push(newPos);
        }
    }
}

bool EnhancedAI::isValidTarget(const Position& pos) const {
    // Check if position is within bounds and not already targeted
    if (pos.x < 0 || pos.x >= GRID_SIZE || pos.y < 0 || pos.y >= GRID_SIZE) {
        return false;
    }
    
    // Check if already hit or missed
    for (const auto& hit : hitPositions) {
        if (hit == pos) return false;
    }
    for (const auto& miss : missPositions) {
        if (miss == pos) return false;
    }
    
    return true;
}

void EnhancedAI::setDifficulty(Difficulty diff) {
    difficulty = diff;
    reset();
}

Difficulty EnhancedAI::getDifficulty() const {
    return difficulty;
}

void EnhancedAI::reset() {
    currentState = AIState::HUNTING;
    hitPositions.clear();
    missPositions.clear();
    possibleTargets.clear();
    targetQueue = std::queue<Position>();
    initializeProbabilityGrid();
}

std::string difficultyToString(Difficulty diff) {
    switch (diff) {
        case Difficulty::EASY: return "Easy";
        case Difficulty::MEDIUM: return "Medium";
        case Difficulty::HARD: return "Hard";
        case Difficulty::EXPERT: return "Expert";
        default: return "Medium";
    }
}

Difficulty stringToDifficulty(const std::string& str) {
    if (str == "Easy") return Difficulty::EASY;
    if (str == "Medium") return Difficulty::MEDIUM;
    if (str == "Hard") return Difficulty::HARD;
    if (str == "Expert") return Difficulty::EXPERT;
    return Difficulty::MEDIUM;
}

} // namespace BattleshipAI