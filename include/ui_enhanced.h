/**
 * Enhanced UI System with Colors, Animations, and Better Visuals
 */

#pragma once
#include "game.h"
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace BattleshipUI {

class ColorConsole {
private:
#ifdef _WIN32
    HANDLE hConsole;
    WORD originalAttributes;
#endif

public:
    ColorConsole();
    ~ColorConsole();
    
    void setColor(int color);
    void resetColor();
    
    // Color constants
    static const int RED = 12;
    static const int GREEN = 10;
    static const int BLUE = 9;
    static const int YELLOW = 14;
    static const int CYAN = 11;
    static const int MAGENTA = 13;
    static const int WHITE = 15;
    static const int GRAY = 8;
};

class EnhancedDisplay {
private:
    ColorConsole console;
    
public:
    EnhancedDisplay();
    
    // Grid display with colors
    void displayGrid(const Grid& grid, bool showShips = false);
    void displayDoubleGrid(const Grid& playerGrid, const Grid& enemyGrid);
    
    // Animated effects
    void showExplosion(const Position& pos);
    void showSplash(const Position& pos);
    void showShipSinking(const Ship& ship);
    void showVictoryAnimation(const std::string& winnerName);
    void showDefeatAnimation();
    
    // Status displays
    void showGameStatus(const Game& game);
    void showPlayerStats(const Player& player);
    void showRemainingShips(const std::vector<Ship>& ships);
    
    // Menu system
    int showMainMenu();
    int showDifficultyMenu();
    int showGameModeMenu();
    int showShipPlacementMenu();
    
    // Utility functions
    void clearScreen();
    void pauseForEffect(int milliseconds);
    void typeWriterEffect(const std::string& text, int delayMs = 50);
};

class ASCIIArt {
public:
    static void displayBattleshipLogo();
    static void displayShipArt(ShipType type);
    static void displayExplosionArt();
    static void displayWavePattern();
    static void displayVictoryBanner();
    static void displayDefeatBanner();
};

} // namespace BattleshipUI