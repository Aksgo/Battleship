/**
 * Configuration Manager Implementation
 * Handles JSON configuration, save/load games, and high scores
 */

#include "config_manager.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <ctime>

namespace Config {

ConfigManager::ConfigManager() : configPath("config/default.json") {
    setDefaultConfig("classic");
}

bool ConfigManager::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open config file " << filename << std::endl;
        return false;
    }

    // Simple JSON parsing (in real implementation, use a proper JSON library)
    std::string line;
    while (std::getline(file, line)) {
        // Parse basic JSON structure
        if (line.find("grid_size") != std::string::npos) {
            // Extract grid size value
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                currentConfig.gridSize = std::stoi(line.substr(pos + 1));
            }
        }
        // Add more JSON parsing as needed
    }

    file.close();
    std::cout << "Loaded configuration from " << filename << std::endl;
    return true;
}

bool ConfigManager::saveConfig(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Simple JSON writing
    file << "{\n";
    file << "  \"grid_size\": " << currentConfig.gridSize << ",\n";
    file << "  \"max_turns\": " << currentConfig.maxTurns << ",\n";
    
    file << "  \"ships\": [\n";
    for (size_t i = 0; i < currentConfig.ships.size(); ++i) {
        const auto& ship = currentConfig.ships[i];
        file << "    {\n";
        file << "      \"name\": \"" << ship.name << "\",\n";
        file << "      \"size\": " << ship.size << ",\n";
        file << "      \"count\": " << ship.count << "\n";
        file << "    }";
        if (i < currentConfig.ships.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n";
    file << "}\n";

    file.close();
    return true;
}

void ConfigManager::setDefaultConfig(const std::string& preset) {
    if (preset == "classic") {
        currentConfig = defaultClassicConfig();
    } else if (preset == "quick") {
        currentConfig = defaultQuickConfig();
    } else if (preset == "large") {
        currentConfig = defaultLargeConfig();
    }
}

GameConfig ConfigManager::defaultClassicConfig() {
    GameConfig config;
    config.gridSize = 10;
    config.maxTurns = 100;
    
    config.ships = {
        {"Carrier", ShipType::CARRIER, 5, 'C', 1},
        {"Battleship", ShipType::BATTLESHIP, 4, 'B', 1},
        {"Destroyer", ShipType::DESTROYER, 3, 'D', 2},
        {"Submarine", ShipType::SUBMARINE, 3, 'S', 1},
        {"Patrol Boat", ShipType::PATROL, 2, 'P', 2}
    };
    
    return config;
}

GameConfig ConfigManager::defaultQuickConfig() {
    GameConfig config;
    config.gridSize = 8;
    config.maxTurns = 50;
    
    config.ships = {
        {"Frigate", ShipType::DESTROYER, 3, 'F', 3},
        {"Gunboat", ShipType::PATROL, 2, 'G', 4}
    };
    
    return config;
}

GameConfig ConfigManager::defaultLargeConfig() {
    GameConfig config;
    config.gridSize = 12;
    config.maxTurns = 150;
    
    config.ships = {
        {"Super Carrier", ShipType::CARRIER, 6, 'C', 1},
        {"Battleship", ShipType::BATTLESHIP, 4, 'B', 2},
        {"Cruiser", ShipType::DESTROYER, 3, 'R', 3},
        {"Patrol", ShipType::PATROL, 2, 'P', 4}
    };
    
    return config;
}

// Getters
GameConfig ConfigManager::getCurrentConfig() const {
    return currentConfig;
}

int ConfigManager::getGridSize() const {
    return currentConfig.gridSize;
}

std::vector<ShipConfig> ConfigManager::getShips() const {
    return currentConfig.ships;
}

std::string ConfigManager::getSetting(const std::string& key) const {
    auto it = currentConfig.settings.find(key);
    return it != currentConfig.settings.end() ? it->second : "";
}

// Setters
void ConfigManager::setGridSize(int size) {
    currentConfig.gridSize = size;
}

void ConfigManager::addShip(const ShipConfig& ship) {
    currentConfig.ships.push_back(ship);
}

void ConfigManager::setSetting(const std::string& key, const std::string& value) {
    currentConfig.settings[key] = value;
}

bool ConfigManager::validateConfig() const {
    if (currentConfig.gridSize < 5 || currentConfig.gridSize > 20) {
        return false;
    }
    
    int totalShipCells = 0;
    for (const auto& ship : currentConfig.ships) {
        totalShipCells += ship.size * ship.count;
    }
    
    // Ensure ships can fit on grid
    return totalShipCells <= (currentConfig.gridSize * currentConfig.gridSize) / 2;
}

// Save Game Manager Implementation
SaveGameManager::SaveGameManager() : saveDirectory("saves/") {
    // Create saves directory if it doesn't exist
    std::filesystem::create_directories(saveDirectory);
}

bool SaveGameManager::saveGame(const Game& game, const std::string& filename) {
    std::ofstream file(saveDirectory + filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Save game state (simplified)
    file << "GameState:\n";
    file << "Turn:" << game.getCurrentTurn() << "\n";
    // Add more game state saving as needed
    
    file.close();
    return true;
}

bool SaveGameManager::loadGame(const std::string& filename, Game& game) {
    std::ifstream file(saveDirectory + filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Load game state (simplified)
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("Turn:") != std::string::npos) {
            // Parse turn count
        }
        // Add more loading logic
    }
    
    file.close();
    return true;
}

std::vector<std::string> SaveGameManager::getSaveFiles() const {
    std::vector<std::string> saves;
    for (const auto& entry : std::filesystem::directory_iterator(saveDirectory)) {
        if (entry.is_regular_file()) {
            saves.push_back(entry.path().filename().string());
        }
    }
    return saves;
}

// High Score Manager Implementation
HighScoreManager::HighScoreManager() : scoreFile("scores.txt") {
    loadScores();
}

void HighScoreManager::addScore(const HighScore& score) {
    highScores.push_back(score);
    
    // Sort by score (descending)
    std::sort(highScores.begin(), highScores.end(), 
              [](const HighScore& a, const HighScore& b) {
                  return a.score > b.score;
              });
    
    // Keep only top 100 scores
    if (highScores.size() > 100) {
        highScores.resize(100);
    }
    
    saveScores();
}

std::vector<HighScore> HighScoreManager::getTopScores(int count) const {
    if (count > highScores.size()) {
        count = highScores.size();
    }
    return std::vector<HighScore>(highScores.begin(), highScores.begin() + count);
}

bool HighScoreManager::loadScores() {
    std::ifstream file(scoreFile);
    if (!file.is_open()) {
        return false;
    }
    
    highScores.clear();
    HighScore score;
    std::string line;
    
    while (std::getline(file, line)) {
        // Parse score line: name,score,turns,date,mode
        // Implementation depends on file format
    }
    
    file.close();
    return true;
}

bool HighScoreManager::saveScores() {
    std::ofstream file(scoreFile);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& score : highScores) {
        file << score.playerName << ","
             << score.score << ","
             << score.turns << ","
             << score.date << ","
             << score.gameMode << "\n";
    }
    
    file.close();
    return true;
}

int HighScoreManager::calculateScore(const Game& game) const {
    // Simple scoring formula
    int baseScore = 1000;
    int turnPenalty = game.getCurrentTurn() * 10;
    int shipBonus = game.getRemainingShips() * 100;
    
    return baseScore - turnPenalty + shipBonus;
}

} // namespace Config