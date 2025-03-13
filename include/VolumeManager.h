#ifndef VOLUME_MANAGER_H
#define VOLUME_MANAGER_H

#include "AnalogVolumeController.h"
#include "Audio.h"

class VolumeManager
{
private:
    AnalogVolumeController &volumeController;
    Audio &audio;
    unsigned long lastUpdate = 0;
    int lastVolume = -1;
    const unsigned long updateInterval = 100; // 100ms update interval

public:
    VolumeManager(AnalogVolumeController &vc, Audio &aud)
        : volumeController(vc), audio(aud) {}

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