#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <map>
#include <string>
#include <fstream>
#include <pigpio.h>
#include <ncurses.h>
#include <unordered_map>
#include <stdexcept>
#include <random>
#include <chrono>
#include <thread>
#include <string>

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

    int lightUpRandomLED(int currentLedPin, std::unordered_map<char, int>& keyToLedMap) {
        // Random number generator setup
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, keyToLedMap.size() - 1);
        int randomIndex = distrib(gen);
        for (auto &led : keyToLedMap) {
            if (randomIndex-- == 0) {
                currentLedPin = led.second;
                break;
            }
        }
        return currentLedPin;
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
        std::ifstream inputFile("highScores.txt");
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
        std::ofstream outputFile("highScores.txt", std::ios::app);
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
    
    void setup(std::unordered_map<char, int>& keyToLedMap) {
        if (gpioInitialise() < 0) {
            throw std::runtime_error("Failed to initialize pigpio.");
        }

        for (auto const& kvp : keyToLedMap) {
            gpioSetMode(kvp.second, PI_OUTPUT);
            gpioWrite(kvp.second, 0); // Initially turn off all LEDs
        }
    }
    
    void inGame(Player& player, HighScore& highScore, std::unordered_map<char, int>& keyToLedMap)
    {
        // Game logic
        // Initialize ncurses mode
        initscr();
        echo(); // Enable echoing of characters
        printw("Enter your username: ");
        char str[100];
        getnstr(str, 99); // Read the username from the user
        player.setName(str);
        noecho(); // Disable echoing of characters for the game
        clear(); // Clear the screen before starting the game

        try {
            setup(keyToLedMap);
            cbreak();
            keypad(stdscr, TRUE); // Enables keypad mode to capture keypresses
            nodelay(stdscr, TRUE); // Non-blocking getch call
            curs_set(0); // Hide cursor

            auto startTime = std::chrono::system_clock::now();
            auto endTime = startTime + std::chrono::seconds(30);
            int timeLeft = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count());
            bool ledOn = false;
            int currentLedPin = -1;
            int score = 0; // Initialize score
            
            // Main game loop
            while (true) {
                auto currentTime = std::chrono::system_clock::now();
                timeLeft = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(endTime - currentTime).count());

                // Break the loop if the time is up
                if (timeLeft < 0) {
                    break;
                }

                // Display time left and score
                mvprintw(0, 0, "Time left: %d seconds ", timeLeft);
                mvprintw(1, 0, "Score: %d", score);
                refresh();

                if (!ledOn) {
                    // Turn off the previous LED
                    if (currentLedPin != -1) {
                        gpioWrite(currentLedPin, 0);
                    }
                    
                    // Turn on a new random LED
                    currentLedPin = ledMatrix.lightUpRandomLED(currentLedPin,keyToLedMap);
                    gpioWrite(currentLedPin, 1);
                    ledOn = true;
                }

                int ch = getch();
                if (ch != ERR) {
                    if (keyToLedMap.count(ch) > 0) {
                        if (keyToLedMap[ch] == currentLedPin) {
                            gpioWrite(currentLedPin, 0); // Turn off the LED
                            ledOn = false;
                            score++; // Increase score for a correct hit
                        } else {
                            score = std::max(0, score - 1); // Decrement score for an incorrect hit without going below zero
                        }
                    }
                }

                // Sleep for a short time to maintain responsiveness
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            // Game over, clean up
            for (auto const& kvp : keyToLedMap) {
                gpioWrite(kvp.second, 0);
            }
            gpioTerminate();

            // Display the final score
            clear();
            mvprintw(0, 0, "Game Over! %s, your score is: %d", str, score);
            refresh();
            nodelay(stdscr, FALSE); // Set getch to blocking mode
            getch(); // Wait for user input before exiting
            endwin();
            
            // After ending ncurses mode, print the final score and username to the console
            std::cout << "Game Over! " << str << ", your final score is: " << score << std::endl;
            highScore.add(score, str);
            
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            endwin();
            gpioTerminate();
        }
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

// Define the pin numbers
const int led1 = 15;
const int led2 = 24;
const int led3 = 8;
const int led4 = 20;
const int led5 = 14;
const int led6 = 23;
const int led7 = 7;
const int led8 = 21;
const int led9 = 3;
const int led10 = 17;
const int led11 = 5;
const int led12 = 19;
const int led13 = 2;
const int led14 = 27;
const int led15 = 6;
const int led16 = 26;

// Map the keys to corresponding LED pins
std::unordered_map<char, int> keyToLedMap = {
    {'4', led1},
    {'5', led2},
    {'6', led3},
    {'7', led4},
    {'r', led5},
    {'t', led6},
    {'y', led7},
    {'u', led8},
    {'f', led9},
    {'g', led10},
    {'h', led11},
    {'j', led12},
    {'v', led13},
    {'b', led14},
    {'n', led15},
    {'m', led16}
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
                game.startGame();
                game.inGame(player, highScore, keyToLedMap);
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
