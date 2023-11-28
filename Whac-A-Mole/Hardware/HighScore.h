#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <map>
#include <string>
#include <vector>
#include <functional> // Include for std::greater

class HighScore {
public:
    void print();
    void add(int score, const std::string& playerName);

    // New method to get high scores
    std::vector<std::pair<int, std::string>> getHighScores() const;

private:
    // Use std::greater to sort scores in descending order
    std::multimap<int, std::string, std::greater<int>> highScoresMap;
};

#endif // HIGHSCORE_H
