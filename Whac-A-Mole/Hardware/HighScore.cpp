#include "HighScore.h"
#include <fstream>
#include <iostream>

std::vector<std::pair<int, std::string>> HighScore::getHighScores() const {
    std::vector<std::pair<int, std::string>> scores;
    for (const auto& entry : highScoresMap) {
        scores.push_back({entry.first, entry.second});
    }
    return scores;
}


void HighScore::print() {
    std::ifstream inputFile("highScores.txt");
    if (inputFile.is_open()) {
        std::string name;
        int score;
        while (inputFile >> name >> score) {
            highScoresMap.insert(std::make_pair(score, name));
        }
        inputFile.close();
        
        std::cout << "Name " << "Score" << std::endl;
        for (auto rit = highScoresMap.rbegin(); rit != highScoresMap.rend(); ++rit) {
            std::cout << rit->second << " " << rit->first << std::endl;
        }
        
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}

void HighScore::add(int score, const std::string& playerName) {
    std::ofstream outputFile("highScores.txt", std::ios::app);
    if (outputFile.is_open()) {
        outputFile << playerName << " " << score << std::endl;
        outputFile.close();
    } else {
        std::cerr << "Unable to open the file for writing." << std::endl;
    }
}
