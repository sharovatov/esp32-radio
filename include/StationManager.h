#ifndef STATION_MANAGER_H
#define STATION_MANAGER_H

#include "AnalogStationController.h"
#include "Audio.h"
#include "esp32-hal-log.h"
#include "LCD.h"
#define TAG "" // logging tag

class StationManager
{
private:
    AnalogStationController &stationController;
    Audio &audio;
    LCD &lcd;
    unsigned long lastUpdate = 0;
    int lastStationNumber = 0; // default is classics

    static constexpr int stationCount = 5;
    const char *radioStations[stationCount] = {
        "https://2.mystreaming.net:443/uber/crchopin/icecast.audio",
        "https://stream.live.vc.bbcmedia.co.uk/bbc_world_service",
        "http://das-edge27-sa23-lax02.cdnstream.com/1078_128?fromyp=true",
        "http://ice1.somafm.com/u80s-128-mp3",
        "https://ic.radiomonster.fm/dance.ultra"};

    const char *stationNames[stationCount] = {
        "Classical",
        "BBC World",
        "Jazz",
        "80s Hits",
        "Trance"};

    const unsigned long updateInterval = 100; // 100ms update interval

public:
    StationManager(AnalogStationController &vc, Audio &aud, LCD &lcdScreen)
        : stationController(vc), audio(aud), lcd(lcdScreen) {}

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

                    lcd.print("Playing:", stationNames[stationNumber]);

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