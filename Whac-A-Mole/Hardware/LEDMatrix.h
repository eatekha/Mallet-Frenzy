#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <unordered_map>
#include <random>

class LEDMatrix {
public:
    LEDMatrix();
    int lightUpRandomLED(int currentLedPin);
    const std::unordered_map<char, int>& getKeyToLedMap() const;

private:
    std::unordered_map<char, int> keyToLedMap;
};

#endif // LEDMATRIX_H
