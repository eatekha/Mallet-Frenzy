#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Timer {
private:
    int timeLeft;
public:
    Timer() : timeLeft(30) {}

    void start() {
        // Start the countdown
        std::cout << "Timer started!\n";
    }

    void stop() {
        // Stop the countdown
        std::cout << "Timer stopped!\n";
    }

    int getTimeLeft() const {
        return timeLeft;
    }
};

class LED {
private:
    bool status;
public:
    LED() : status(false) {}

    void turnOn() {
        status = true;
    }

    void turnOff() {
        status = false;
    }

    bool isOn() const {
        return status;
    }
};

class LEDMatrix {
private:
    std::vector<LED> leds;
public:
    LEDMatrix() : leds(16) {}

    LED& lightUpRandomLED() {
        int index = rand() % leds.size();
        leds[index].turnOn();
        return leds[index];
    }
};

class Player {
private:
    std::string name;
    int score;
public:
    Player() : score(0) {}

    void setName(const std::string& n) {
        name = n;
    }

    int getScore() const {
        return score;
    }
};

class GameController {
private:
    int currentScore;
    Timer timer;
    LEDMatrix ledMatrix;
    Player currentPlayer;
public:
    GameController() : currentScore(0) {}

    void startGame() {
        std::cout << "Game started!\n";
        timer.start();
    }

    void endGame() {
        std::cout << "Game ended!\n";
        timer.stop();
        std::cout << "Final score: " << currentScore << "\n";
    }

    void incrementScore() {
        currentScore += 10;  
    }
};

int main() {
    GameController game;
    game.startGame();

    // Game Logic

    game.endGame();
    return 0;
}
