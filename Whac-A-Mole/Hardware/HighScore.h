#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <map>
#include <string>

class HighScore {
public:
    void print();
    void add(int score, const std::string& playerName);

private:
    std::multimap<int, std::string> highScoresMap;
};

#endif // HIGHSCORE_H
