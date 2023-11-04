#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>


using namespace std;
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
    srand(static_cast<unsigned int>(time(0))); // Seed for randomness
    GameController game; // Instantiate GameController
    Player player;
    string playerName; // Declare playerName variable
    int choice; // Declare choice variable
    
    bool running = true;
    while (running) {
        cout << "Whack-a-LED Game\n";
        cout << "1. Start Game\n";
        cout << "2. Instructions\n";
        cout << "3. High Scores\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter player name: ";
                cin >> playerName;
                player.setName(playerName);
                
                game.startGame();
                
                // Game logic goes here
                game.endGame();
                break;
            case 2:
                cout << "Instructions:\n";
                cout << "- The LED matrix will light up in random patterns.\n";
                cout << "- Your goal is to 'hit' the lit LED by pressing the enter key as soon as it lights up.\n";
                cout << "- The faster you hit, the more points you score.\n";
                cout << "- You have a limited amount of time to score as many points as possible.\n\n";
                break;
            case 3:
                cout << "High Scores: \n";
                // Code to display high scores goes here
                break;
            case 4:
                running = false;
                break;
            default:
                cout << "Invalid choice, please try again.\n";
                break;
        }

        // Clear the input buffer in case of invalid input
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining input
        }
    }
        

    return 0;
}
