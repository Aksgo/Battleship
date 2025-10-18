/**
 * Additional Game Modes for Battleship
 * Features: Salvo, Mine Warfare, Fog of War, Time Attack
 */

#pragma once
#include "game.h"
#include <vector>
#include <chrono>

namespace BattleshipModes {

class GameMode {
protected:
    std::string name;
    std::string description;
    
public:
    GameMode(const std::string& name, const std::string& desc);
    virtual ~GameMode() = default;
    
    virtual void initialize(Game& game) = 0;
    virtual void onTurnStart(Player& player) = 0;
    virtual void onTurnEnd(Player& player) = 0;
    virtual bool checkWinCondition(const Game& game) = 0;
    
    std::string getName() const;
    std::string getDescription() const;
};

// Salvo Mode: Fire multiple shots per turn based on remaining ships
class SalvoMode : public GameMode {
private:
    int baseShots;
    
public:
    SalvoMode(int shots = 1);
    void initialize(Game& game) override;
    void onTurnStart(Player& player) override;
    void onTurnEnd(Player& player) override;
    bool checkWinCondition(const Game& game) override;
    
    int getShotsForPlayer(const Player& player) const;
};

// Mine Warfare: Add mines to the battlefield
class MineWarfareMode : public GameMode {
private:
    std::vector<Position> minePositions;
    int mineCount;
    
public:
    MineWarfareMode(int mines = 5);
    void initialize(Game& game) override;
    void onTurnStart(Player& player) override;
    void onTurnEnd(Player& player) override;
    bool checkWinCondition(const Game& game) override;
    
    bool checkMineHit(const Position& pos) const;
    void triggerMine(Player& player, const Position& pos);
};

// Fog of War: Limited visibility around shots
class FogOfWarMode : public GameMode {
private:
    int visibilityRange;
    
public:
    FogOfWarMode(int range = 2);
    void initialize(Game& game) override;
    void onTurnStart(Player& player) override;
    void onTurnEnd(Player& player) override;
    bool checkWinCondition(const Game& game) override;
    
    Grid getVisibleGrid(const Player& player, const Grid& enemyGrid) const;
};

// Time Attack: Limited time per turn
class TimeAttackMode : public GameMode {
private:
    std::chrono::seconds timePerTurn;
    
public:
    TimeAttackMode(int seconds = 30);
    void initialize(Game& game) override;
    void onTurnStart(Player& player) override;
    void onTurnEnd(Player& player) override;
    bool checkWinCondition(const Game& game) override;
    
    bool isTimeUp(const std::chrono::steady_clock::time_point& startTime) const;
};

// Game Mode Manager
class GameModeManager {
private:
    std::vector<std::unique_ptr<GameMode>> availableModes;
    GameMode* currentMode;
    
public:
    GameModeManager();
    
    void registerMode(std::unique_ptr<GameMode> mode);
    void setCurrentMode(const std::string& modeName);
    GameMode* getCurrentMode() const;
    std::vector<std::string> getAvailableModeNames() const;
    
    void initializeCurrentMode(Game& game);
};

} // namespace BattleshipModes