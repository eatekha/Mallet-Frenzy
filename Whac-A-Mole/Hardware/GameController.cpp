#include "GameController.h"
#include <pigpio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <ncurses.h>
GameController::GameController() : timer(), ledMatrix(), currentPlayer() {}

void GameController::setup() {
    if (gpioInitialise() < 0) {
        throw std::runtime_error("Failed to initialize pigpio.");
    }
    const auto& keyToLedMap = ledMatrix.getKeyToLedMap();
    for (auto const& kvp : keyToLedMap) {
        gpioSetMode(kvp.second, PI_OUTPUT);
        gpioWrite(kvp.second, 0); // Initially turn off all LEDs
    }
}

void GameController::startGame() {
    std::cout << "Game started!\n";
    timer.start();
}

void GameController::inGame(Player& player, HighScore& highScore) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    int currentLedPin = -1;
    bool ledOn = false;

    while (!timer.isTimeUp()) {
        if (!ledOn) {
            if (currentLedPin != -1) {
                gpioWrite(currentLedPin, 0);
            }
            currentLedPin = ledMatrix.lightUpRandomLED(currentLedPin);
            ledOn = true;
        }

        int ch = getch();
        if (ch != ERR) {
            const auto& keyToLedMap = ledMatrix.getKeyToLedMap();
            if (keyToLedMap.count(ch) > 0) {
                if (keyToLedMap.at(ch) == currentLedPin) {
                    gpioWrite(currentLedPin, 0);
                    ledOn = false;
                    player.incrementScore();
                } else {
                    player.decrementScore();
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    endwin();
    gpioTerminate();
    std::cout << "Game Over! Your score is: " << player.getScore() << std::endl;
    highScore.add(player.getScore(), player.getName());
}

void GameController::endGame(Player& player) {
    std::cout << "Game ended!\n";
    timer.stop();
    std::cout << "Final score: " << player.getScore() << "\n";
}
