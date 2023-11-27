#include "LEDMatrix.h"
#include <pigpio.h>

LEDMatrix::LEDMatrix() {
    // Initialize LED pin numbers
    keyToLedMap = {
        {'4', 15}, {'5', 24}, {'6', 8}, {'7', 20},
        {'r', 14}, {'t', 23}, {'y', 7}, {'u', 21},
        {'f', 3}, {'g', 17}, {'h', 5}, {'j', 19},
        {'v', 2}, {'b', 27}, {'n', 6}, {'m', 26}
    };
}

int LEDMatrix::lightUpRandomLED(int currentLedPin) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, keyToLedMap.size() - 1);
    int randomIndex = distrib(gen);
    for (auto &led : keyToLedMap) {
        if (randomIndex-- == 0) {
            currentLedPin = led.second;
            gpioWrite(currentLedPin, 1); // Turn on the new LED
            break;
        }
    }
    return currentLedPin;
}

const std::unordered_map<char, int>& LEDMatrix::getKeyToLedMap() const {
    return keyToLedMap;
}
