/**********************************************************************
  Description : The simplest internet radio. The app connects to wifi
                and streams one of the five hardcoded stations.
  Author      : Vitaly Sharovatov
**********************************************************************/
#include <WiFi.h>
#include "Audio.h"
#include "LCD.h"
#include "secrets.h"
#include "VolumeManager.h"
#include "StationManager.h"
#include "PotReader.h"

#include "esp32-hal-log.h"
#define TAG "" // logging tag

String wifi_init()
{
  const char *ssid_Router = WIFI_SSID;
  const char *password_Router = WIFI_PASS;

  WiFi.begin(ssid_Router, password_Router);
  WiFi.setSleep(false);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30)
  {
    delay(500);
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    ESP_LOGE(TAG, "\nFailed to connect to WiFi.");
    return "";
  }

  return WiFi.localIP().toString();
}

Audio audio;

void audio_init()
{

  constexpr int I2S_BCLK = 26;
  constexpr int I2S_LRC = 25;
  constexpr int I2S_DOUT = 32;

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  // the volume level will be set in the loop() func upon the first time when the pot value is read

  // Play an online radio stream
  // audio.connecttohost("https://2.mystreaming.net:443/uber/crchopin/icecast.audio");
}

LCD lcd;

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  ESP_LOGI(TAG, "Entering Setup function, heap size = %d bytes, starting LCD init", ESP.getFreeHeap());

  lcd.begin();

  ESP_LOGI(TAG, "Heap after LCD init: %d bytes, starting WiFi init\n", ESP.getFreeHeap());

  String wifi_ip_address = wifi_init();
  if (wifi_ip_address.isEmpty())
  {
    ESP_LOGE(TAG, "Wifi init failed! Stopping...");
    lcd.print("No WiFi :(");
    return;
  }

  ESP_LOGI(TAG, "Done WIFI init, heap size = %d bytes, WiFi addr = %s", ESP.getFreeHeap(), wifi_ip_address.c_str());

  audio_init();

  ESP_LOGI(TAG, "Done audio init, heap size = %d bytes\n", ESP.getFreeHeap());
  lcd.print("Initializing...");
}

// Volume potentiometer on GPIO 33, mapped to 0-21 for volume values
constexpr int volumePotPin = 33;
PotReader volumeReader(volumePotPin, 21);
VolumeManager volumeManager(volumeReader, audio, lcd);

// Station potentiometer on GPIO 36, mapped to 0-4 for station numbers
constexpr int stationsPotPin = 36;
PotReader stationReader(stationsPotPin, 4);
StationManager stationManager(stationReader, audio, lcd);

void loop()
{
  // keep streaming the audio
  audio.loop();

  // keep reading the volume pot and update the volume when needed
  volumeManager.update();

  // keep reading the stations pot and update the station when needed
  stationManager.update();
}
