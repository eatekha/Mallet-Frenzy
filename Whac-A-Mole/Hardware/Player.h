#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    Player();
    void setName(const std::string& n);
    std::string getName() const;
    int getScore() const;
    void incrementScore();
    void decrementScore();

private:
    std::string name;
    int score;
};

#endif // PLAYER_H
