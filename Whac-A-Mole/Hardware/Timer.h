#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
    Timer();

    void start();
    void stop();
    int getTimeLeft() const;
    bool isTimeUp() const;

private:
    int timeLeft;
    std::chrono::time_point<std::chrono::system_clock> endTime;
};

#endif // TIMER_H
