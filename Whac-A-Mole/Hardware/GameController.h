#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Timer.h"
#include "LEDMatrix.h"
#include "Player.h"
#include "HighScore.h"
#include <ncurses.h>

class GameController {
public:
    GameController();
    void setup();
    void startGame();
    void inGame(Player& player, HighScore& highScore);
    void endGame(Player& player);

    Timer timer;
    LEDMatrix ledMatrix;
    Player currentPlayer;
};

#endif // GAMECONTROLLER_H
