#include <iostream>
#include <map>
#include <ctime>
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

/**
 * @brief Class representing a countdown timer.
 */
 
class Timer {
private:
    int timeLeft;
    std::chrono::time_point<std::chrono::system_clock> endTime;

public:
    Timer() : timeLeft(30) {}

    void start() 
    {
        endTime = std::chrono::system_clock::now() + std::chrono::seconds(timeLeft);
        std::cout << "Timer started!\n";
    }

    void stop() 
    {
        std::cout << "Timer stopped!\n";
    }

    int getTimeLeft() const 
    {
        auto now = std::chrono::system_clock::now();
        auto durationLeft = std::chrono::duration_cast<std::chrono::seconds>(endTime - now);
        return std::max(0, static_cast<int>(durationLeft.count()));
    }

    bool isTimeUp() const 
    {
        return std::chrono::system_clock::now() >= endTime;
    }
};


/**
 * @brief Class representing a matrix of LEDs.
 */
class LEDMatrix {
private:
    std::unordered_map<char, int> keyToLedMap;

public:
    LEDMatrix() 
    {
        // Initialize LED pin numbers
        keyToLedMap = 
        {
            {'4', 15}, {'5', 24}, {'6', 8}, {'7', 20},
            {'r', 14}, {'t', 23}, {'y', 7}, {'u', 21},
            {'f', 3}, {'g', 17}, {'h', 5}, {'j', 19},
            {'v', 2}, {'b', 27}, {'n', 6}, {'m', 26}
        };
    }

    int lightUpRandomLED(int currentLedPin) 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, keyToLedMap.size() - 1);
        int randomIndex = distrib(gen);
        for (auto &led : keyToLedMap) 
        {
            if (randomIndex-- == 0) 
            {
                currentLedPin = led.second;
                break;
            }
        }
        return currentLedPin;
    }

    const std::unordered_map<char, int>& getKeyToLedMap() const {
        
        return keyToLedMap;
    }
};


/**
 * @brief Class representing a player in the game.
 */
class Player {
private:
    std::string name;
    int score;
public:
    Player() : score(0) {}

    void setName(const std::string& n) 
    {
        name = n;
    }

    std::string getName() const 
    {
        return name;
    }

    int getScore() const 
    {
        return score;
    }

    void incrementScore() 
    {
        score++;
    }

    void decrementScore() 
    {
        if (score > 0) {
            score--;
        }
    }
};



/**
 * @brief Class for managing high scores.
 */
class HighScore {
    //Ordered Map
    std::multimap<int, std::string > highScoresMap;
private:
    int score;
    std::string name;
    int count;

public:
    /**
     * @brief Prints the top 5 high scores.
     */
    void print() 
    {
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
    
    /**
     * @brief Adds a new high score to the file.
     * 
     * @param score The score to add.
     * @param playerName The name of the player.
     */
    void add(int score, std::string playerName) 
    {
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

/**
 * @brief Controller class for the game.
 */
class GameController {
private:
    Timer timer;
    LEDMatrix ledMatrix;
    Player currentPlayer;
public:
    
    /**
     * @brief Starts the game, initializes the timer.
     */
    void startGame() 
    {
        std::cout << "Game started!\n";
        timer.start();
    }
    
    /**
     * @brief Sets up the RasberryPI in order to get ready to run the game
     */
    void setup() 
    {
        if (gpioInitialise() < 0) 
        {
            throw std::runtime_error("Failed to initialize pigpio.");
        }

        const auto& keyToLedMap = ledMatrix.getKeyToLedMap();
        for (auto const& kvp : keyToLedMap) 
        {
            gpioSetMode(kvp.second, PI_OUTPUT);
            gpioWrite(kvp.second, 0); // Initially turn off all LEDs
        }
    }

    
    /**
     * @brief Main logic of the Wac A Mole Game
     */
    void inGame(Player& player, HighScore& highScore) {
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
        setup(); // Setup using LEDMatrix
        cbreak();
        keypad(stdscr, TRUE); // Enables keypad mode to capture keypresses
        nodelay(stdscr, TRUE); // Non-blocking getch call
        curs_set(0); // Hide cursor

        timer.start(); // Start the timer
        bool ledOn = false;
        int currentLedPin = -1;
        
        // Main game loop
        while (!timer.isTimeUp()) 
        {
            // Display time left and player's score
            mvprintw(0, 0, "Time left: %d seconds ", timer.getTimeLeft());
            mvprintw(1, 0, "Score: %d", player.getScore());
            refresh();

            if (!ledOn) {
                // Turn off the previous LED
                if (currentLedPin != -1) {
                    gpioWrite(currentLedPin, 0);
                }
                
                // Turn on a new random LED
                currentLedPin = ledMatrix.lightUpRandomLED(currentLedPin);
                gpioWrite(currentLedPin, 1);
                ledOn = true;
            }

            int ch = getch();
            if (ch != ERR) {
                const auto& keyToLedMap = ledMatrix.getKeyToLedMap();
                if (keyToLedMap.count(ch) > 0) {
                    if (keyToLedMap.at(ch) == currentLedPin) {
                        gpioWrite(currentLedPin, 0); // Turn off the LED
                        ledOn = false;
                        player.incrementScore(); // Increase player's score for a correct hit
                    } else {
                        player.decrementScore(); // Decrement player's score for an incorrect hit
                    }
                }
            }

            // Sleep for a short time to maintain responsiveness
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Game over, clean up
        const auto& keyToLedMap = ledMatrix.getKeyToLedMap();
        for (auto const& kvp : keyToLedMap) 
        {
            gpioWrite(kvp.second, 0);
        }
        gpioTerminate();

        // Display the final score
        clear();
        mvprintw(0, 0, "Game Over! %s, your score is: %d", player.getName().c_str(), player.getScore());
        refresh();
        nodelay(stdscr, FALSE); // Set getch to blocking mode
        getch(); // Wait for user input before exiting
        endwin();
        
        // After ending ncurses mode, print the final score and username to the console
        std::cout << "Game Over! " << player.getName() << ", your final score is: " << player.getScore() << std::endl;
        highScore.add(player.getScore(), player.getName());
        
    } catch (const std::runtime_error& e) 
    {
        std::cerr << e.what() << std::endl;
        endwin();
        gpioTerminate();
    }
}



    
    /**
     * @brief End message when the game is over
     */
    void endGame(Player& player) 
    {
        std::cout << "Game ended!\n";
        timer.stop();
        std::cout << "Final score: " << player.getScore() << "\n";
    }
};


/**
 * @brief Main entry point for the Whack-a-LED game.
 * 
 * @return int Returns 0 upon successful execution.
 */
int main() {
    HighScore highScore;
    srand(static_cast<unsigned int>(time(0))); // Seed for randomness
    GameController game; // Instantiate GameController
    Player player;
    int choice; // Declare choice variable

    bool running = true;
    while (running) 
    {
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
                game.inGame(player, highScore);
                game.endGame(player);

                break;
            case 2:
                cout << "Instructions:\n";
                cout << "- The LED matrix will light up in random patterns.\n";
                cout << "- Your goal is to 'hit' the lit LED by pressing the enter key as soon as it lights up.\n";
                cout << "- The faster you hit, the more points you score.\n";
                cout << "- The keybinds for the 4x4 matrix are set as follows:\n";
                cout << "- 4  5  6  7\n";
                cout << "- r  t  y  u\n";
                cout << "- f  g  h  j\n";
                cout << "- v  b  n  m\n";
                cout << "- You have 30 seconds to score as many points as possible.\n\n";

                break;
            case 3:
                cout << "High Scores: \n";
                highScore.print();
                getch();

                break;
            case 4:
                running = false;
                break;
            default:
                cout << "Invalid choice, please try again.\n";

                break;
        }

        // Clear the input buffer in case of invalid input
        if (cin.fail()) 
        {
            cin.clear(); // Clear the error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining input
        }
        
    }


    return 0;
}
