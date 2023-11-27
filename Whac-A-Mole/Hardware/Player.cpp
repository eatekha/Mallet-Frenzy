#include "Player.h"

Player::Player() : score(0) {}

void Player::setName(const std::string& n) {
    name = n;
}

std::string Player::getName() const {
    return name;
}

int Player::getScore() const {
    return score;
}

void Player::incrementScore() {
    score++;
}

void Player::decrementScore() {
    if (score > 0) {
        score--;
    }
}
