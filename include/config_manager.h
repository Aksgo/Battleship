/**
 * Configuration Management System for Battleship
 * Features: JSON configuration, save/load settings, custom ship sets
 */

#pragma once
#include "game.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

// Simple JSON parser for configuration
namespace Config {

struct ShipConfig {
    std::string name;
    ShipType type;
    int size;
    char symbol;
    int count;
};

struct GameConfig {
    int gridSize;
    int maxTurns;
    std::vector<ShipConfig> ships;
    std::map<std::string, std::string> settings;
};

class ConfigManager {
private:
    GameConfig currentConfig;
    std::string configPath;
    
    // Default configurations
    GameConfig defaultClassicConfig();
    GameConfig defaultQuickConfig();
    GameConfig defaultLargeConfig();
    
public:
    ConfigManager();
    
    // Configuration management
    bool loadConfig(const std::string& filename);
    bool saveConfig(const std::string& filename) const;
    void setDefaultConfig(const std::string& preset = "classic");
    
    // Getters
    GameConfig getCurrentConfig() const;
    int getGridSize() const;
    std::vector<ShipConfig> getShips() const;
    std::string getSetting(const std::string& key) const;
    
    // Setters
    void setGridSize(int size);
    void addShip(const ShipConfig& ship);
    void setSetting(const std::string& key, const std::string& value);
    
    // Validation
    bool validateConfig() const;
};

// Save Game System
class SaveGameManager {
private:
    std::string saveDirectory;
    
public:
    SaveGameManager();
    
    bool saveGame(const Game& game, const std::string& filename);
    bool loadGame(const std::string& filename, Game& game);
    
    std::vector<std::string> getSaveFiles() const;
    bool deleteSaveFile(const std::string& filename);
    
    std::string getSaveInfo(const std::string& filename) const;
};

// High Score System
struct HighScore {
    std::string playerName;
    int score;
    int turns;
    std::string date;
    std::string gameMode;
};

class HighScoreManager {
private:
    std::vector<HighScore> highScores;
    std::string scoreFile;
    
public:
    HighScoreManager();
    
    void addScore(const HighScore& score);
    std::vector<HighScore> getTopScores(int count = 10) const;
    std::vector<HighScore> getScoresByMode(const std::string& mode) const;
    
    bool loadScores();
    bool saveScores();
    
    int calculateScore(const Game& game) const;
};

} // namespace Config