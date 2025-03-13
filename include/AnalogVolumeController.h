#include <esp32-hal-adc.h>
#ifndef ANALOG_VOLUME_CONTROLLER_H
#define ANALOG_VOLUME_CONTROLLER_H

class AnalogVolumeController
{
private:
    int potPin;

    // Convert 0-4095 range to 0-21 range
    int mapPotValueToVolume(int potValue)
    {
        return potValue / 186;
    }

public:
    AnalogVolumeController(int pin) : potPin(pin) {}

    // Read current volume (already smooth due to integer division)
    int read()
    {
        return mapPotValueToVolume(analogRead(potPin));
    }
};

#endif // ANALOG_VOLUME_CONTROLLER_H