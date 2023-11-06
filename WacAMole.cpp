#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <map>
#include <string>
#include <fstream>

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

// How we add to print HighScores
class HighScore {
    //Ordered Map
    std::multimap<int, std::string > highScoresMap;
private:
    int score;
    std::string name;
    int count;

public:
    void print() {
        std::ifstream inputFile("../highScores.txt");
        //Iterates through file under schema "number score"
        if (inputFile.is_open()) {
            while (inputFile >> name >> score) {
                highScoresMap.insert(std::make_pair(score, name));
            }
            inputFile.close();
        } else {
            std::cerr << "Unable to open the file." << std::endl;
        }
        //Title
        std::cout << "Name " << "Score" << std::endl;
        // Iterates for the first 5 highest scores
        count = 0;
        for (auto rit = highScoresMap.rbegin(); rit != highScoresMap.rend() && count < 5; rit++) {
            std::cout << rit->second << " " << rit->first << std::endl;
            count++;
        }
        //Clears so it doesn't print duplicates
        highScoresMap.clear();

    }
    //adding to file
    void add(int score, std::string playerName) {
        // Open the file for appending
        std::ofstream outputFile("../highScores.txt", std::ios::app);
        if (outputFile.is_open()) {
            // Write new data to the file
            outputFile << std::endl << playerName << " " << score;
            outputFile.close();
        } else {
            std::cerr << "Unable to open the file for writing." << std::endl;
        }
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
    int getScore () {
        return currentScore;
    }
};

int main() {
    HighScore highScore;
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
                highScore.add(game.getScore(), playerName);
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
                highScore.print();
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
