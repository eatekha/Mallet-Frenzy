#include "Timer.h"
#include <iostream>
#include <algorithm>

Timer::Timer() : timeLeft(30) {}

void Timer::start() {
    endTime = std::chrono::system_clock::now() + std::chrono::seconds(timeLeft);
    std::cout << "Timer started!\n";
}

void Timer::stop() {
    std::cout << "Timer stopped!\n";
}

int Timer::getTimeLeft() const {
    auto now = std::chrono::system_clock::now();
    auto durationLeft = std::chrono::duration_cast<std::chrono::seconds>(endTime - now);
    return std::max(0, static_cast<int>(durationLeft.count()));
}

bool Timer::isTimeUp() const {
    return std::chrono::system_clock::now() >= endTime;
}
