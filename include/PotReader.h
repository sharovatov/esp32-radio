#include <esp32-hal-adc.h>
#ifndef POT_READER_H
#define POT_READER_H

class PotReader
{
private:
    int potPin;
    int maxValue; // Defines the range (e.g., 21 for volume, 4 for stations)

public:
    PotReader(int pin, int maxVal) : potPin(pin), maxValue(maxVal) {}

    // Read and map the value from 0-4095 to 0-maxValue
    int read()
    {
        return constrain(analogRead(potPin) * maxValue / 4095, 0, maxValue);
    }
};

#endif // POT_READER_H