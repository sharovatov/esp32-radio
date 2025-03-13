#ifndef VOLUME_MANAGER_H
#define VOLUME_MANAGER_H

#include "PotReader.h"
#include "Audio.h"

class VolumeManager
{
private:
    PotReader &volumeController;
    Audio &audio;
    LCD &lcd;
    unsigned long lastUpdate = 0;
    int lastVolume = -1;
    const unsigned long updateInterval = 100; // 100ms update interval

public:
    VolumeManager(PotReader &vc, Audio &aud, LCD &lcdScreen)
        : volumeController(vc), audio(aud), lcd(lcdScreen) {}

    void update()
    {
        if (millis() - lastUpdate >= updateInterval)
        {
            lastUpdate = millis();

            int volume = volumeController.read();
            if (volume != lastVolume)
            {
                lastVolume = volume;
                audio.setVolume(volume);
            }
        }
    }
};

#endif // VOLUME_MANAGER_H