#include "ui_enhanced.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace BattleshipUI {

// ColorConsole Implementation
ColorConsole::ColorConsole() {
#ifdef _WIN32
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    originalAttributes = consoleInfo.wAttributes;
#endif
}

ColorConsole::~ColorConsole() {
    resetColor();
}

void ColorConsole::setColor(int color) {
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, color);
#else
    // Linux/Mac color codes (simplified)
    std::cout << "\033[1;" << color << "m";
#endif
}

void ColorConsole::resetColor() {
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, originalAttributes);
#else
    std::cout << "\033[0m";
#endif
}

// EnhancedDisplay Implementation
EnhancedDisplay::EnhancedDisplay() {}

void EnhancedDisplay::displayGrid(const Grid& grid, bool showShips) {
    std::cout << "  ";
    for (int i = 0; i < grid.getSize(); i++) {
        std::cout << " " << i + 1;
    }
    std::cout << "\n";
    
    for (int y = 0; y < grid.getSize(); y++) {
        console.setColor(ColorConsole::CYAN);
        std::cout << char('A' + y) << " ";
        console.resetColor();
        
        for (int x = 0; x < grid.getSize(); x++) {
            Position pos = {x, y};
            CellState state = grid.getCellState(pos);
            
            switch (state) {
                case CellState::EMPTY:
                    console.setColor(ColorConsole::BLUE);
                    std::cout << "~ ";
                    break;
                case CellState::SHIP:
                    if (showShips) {
                        console.setColor(ColorConsole::GREEN);
                        std::cout << "S ";
                    } else {
                        console.setColor(ColorConsole::BLUE);
                        std::cout << "~ ";
                    }
                    break;
                case CellState::HIT:
                    console.setColor(ColorConsole::RED);
                    std::cout << "X ";
                    break;
                case CellState::MISS:
                    console.setColor(ColorConsole::YELLOW);
                    std::cout << "O ";
                    break;
            }
            console.resetColor();
        }
        std::cout << "\n";
    }
}

void EnhancedDisplay::displayDoubleGrid(const Grid& playerGrid, const Grid& enemyGrid) {
    std::cout << "Your Grid";
    for (int i = 0; i < playerGrid.getSize() * 2 - 6; i++) std::cout << " ";
    std::cout << "Enemy Grid\n";
    
    std::cout << "  ";
    for (int i = 0; i < playerGrid.getSize(); i++) std::cout << " " << i + 1;
    std::cout << "    ";
    for (int i = 0; i < enemyGrid.getSize(); i++) std::cout << " " << i + 1;
    std::cout << "\n";
    
    for (int y = 0; y < playerGrid.getSize(); y++) {
        // Player grid
        console.setColor(ColorConsole::CYAN);
        std::cout << char('A' + y) << " ";
        console.resetColor();
        
        for (int x = 0; x < playerGrid.getSize(); x++) {
            displayCell(playerGrid, {x, y}, true);
        }
        
        std::cout << "  ";
        
        // Enemy grid  
        console.setColor(ColorConsole::CYAN);
        std::cout << char('A' + y) << " ";
        console.resetColor();
        
        for (int x = 0; x < enemyGrid.getSize(); x++) {
            displayCell(enemyGrid, {x, y}, false);
        }
        
        std::cout << "\n";
    }
}

void EnhancedDisplay::displayCell(const Grid& grid, const Position& pos, bool showShips) {
    CellState state = grid.getCellState(pos);
    
    switch (state) {
        case CellState::EMPTY:
            console.setColor(ColorConsole::BLUE);
            std::cout << "~ ";
            break;
        case CellState::SHIP:
            if (showShips) {
                console.setColor(ColorConsole::GREEN);
                std::cout << "S ";
            } else {
                console.setColor(ColorConsole::BLUE);
                std::cout << "~ ";
            }
            break;
        case CellState::HIT:
            console.setColor(ColorConsole::RED);
            std::cout << "X ";
            break;
        case CellState::MISS:
            console.setColor(ColorConsole::YELLOW);
            std::cout << "O ";
            break;
    }
    console.resetColor();
}

void EnhancedDisplay::showExplosion(const Position& pos) {
    clearScreen();
    console.setColor(ColorConsole::RED);
    std::cout << "\n        *** BOOM! ***\n";
    std::cout << "    Direct hit at " << char('A' + pos.x) << pos.y + 1 << "!\n\n";
    console.resetColor();
    pauseForEffect(1000);
}

void EnhancedDisplay::showVictoryAnimation(const std::string& winnerName) {
    clearScreen();
    console.setColor(ColorConsole::GREEN);
    
    std::vector<std::string> fireworks = {
        "   \\ /   ",
        "   -+-   ",
        "   / \\   ",
        "         ",
        "  \\   /  ",
        "   ---   ",
        "  /   \\  "
    };
    
    for (const auto& line : fireworks) {
        std::cout << line << "\n";
    }
    
    std::cout << "\n  VICTORY! " << winnerName << " wins!\n\n";
    console.resetColor();
    pauseForEffect(2000);
}

void EnhancedDisplay::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void EnhancedDisplay::pauseForEffect(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void EnhancedDisplay::typeWriterEffect(const std::string& text, int delayMs) {
    for (char c : text) {
        std::cout << c << std::flush;
        pauseForEffect(delayMs);
    }
    std::cout << "\n";
}

int EnhancedDisplay::showMainMenu() {
    clearScreen();
    ASCIIArt::displayBattleshipLogo();
    
    console.setColor(ColorConsole::CYAN);
    std::cout << "\n=== MAIN MENU ===\n";
    console.resetColor();
    
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Settings\n";
    std::cout << "4. Exit\n";
    
    std::cout << "\nSelect option: ";
    int choice;
    std::cin >> choice;
    return choice;
}

// ASCII Art Implementation
void ASCIIArt::displayBattleshipLogo() {
    std::cout << R"(
    ____       __  __       __   _____ __           __  
   / __ )___  / /_/ /_     / /  / ___// /___ ______/ /_ 
  / __  / _ \\/ __/ __ \\   / /   \\__ \\/ / __ `/ ___/ __ \\
 / /_/ /  __/ /_/ / / /  / /______/ / / /_/ / /__/ / / /
/_____/\\___/\\__/_/ /_/  /_____/____/_/\\__,_/\\___/_/ /_/ 
    )" << "\n\n";
}

} // namespace BattleshipUI