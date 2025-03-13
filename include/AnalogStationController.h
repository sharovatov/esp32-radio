#include <esp32-hal-adc.h>
#ifndef ANALOG_STATION_CONTROLLER_H
#define ANALOG_STATION_CONTROLLER_H

class AnalogStationController
{
private:
    int potPin;

    // Convert 0-4095 range to 0-4 range (5 stations)
    int mapPotValueToStation(int potValue)
    {
        // todo: 4 is maxStationsCound!!!
        return constrain(potValue / 820, 0, 4); // Ensures 0-4 range
    }

public:
    AnalogStationController(int pin) : potPin(pin) {}

    // Read current volume (already smooth due to integer division)
    int read()
    {
        return mapPotValueToStation(analogRead(potPin));
    }
};

#endif // ANALOG_STATION_CONTROLLER_H