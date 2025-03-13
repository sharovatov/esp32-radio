#ifndef STATION_MANAGER_H
#define STATION_MANAGER_H

#include "AnalogStationController.h"
#include "Audio.h"
#include "esp32-hal-log.h"
#define TAG "" // logging tag

class StationManager
{
private:
    AnalogStationController &stationController;
    Audio &audio;
    unsigned long lastUpdate = 0;
    int lastStationNumber = 0; // default is classics

    static constexpr int stationCount = 5;
    const char *radioStations[stationCount] = {
        "https://2.mystreaming.net:443/uber/crchopin/icecast.audio",       // classical
        "https://stream.live.vc.bbcmedia.co.uk/bbc_world_service",         // BBC world service
        "http://das-edge27-sa23-lax02.cdnstream.com/1078_128?fromyp=true", // JAZZ
        "http://ice1.somafm.com/u80s-128-mp3",                             // 80s Hits
        "https://ic.radiomonster.fm/dance.ultra"                           // trance
    };

    const unsigned long updateInterval = 100; // 100ms update interval

public:
    StationManager(AnalogStationController &vc, Audio &aud)
        : stationController(vc), audio(aud) {}

    void update()
    {
        if (millis() - lastUpdate >= updateInterval)
        {
            lastUpdate = millis();

            int stationNumber = stationController.read();
            if (stationNumber != lastStationNumber)
            {
                if (stationNumber >= 0 && stationNumber < stationCount)
                {
                    lastStationNumber = stationNumber;

                    audio.stopSong();
                    audio.connecttohost(radioStations[stationNumber]);

                    ESP_LOGI(TAG, "Switched to station: %d — %s", stationNumber, radioStations[stationNumber]);
                }
                else
                {
                    ESP_LOGE(TAG, "Invalid stationNumber: %d", stationNumber);
                }
            }
        }
    }
};

#endif // STATION_MANAGER_H