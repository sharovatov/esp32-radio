/**********************************************************************
  Description : The simplest internet radio. The app connects to wifi
                and streams one station to the headset
  Author      : Vitaly Sharovatov
**********************************************************************/
#include <WiFi.h>
#include "Audio.h"
#include "LCD.h"
#include "secrets.h"

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

#define I2S_BCLK 26
#define I2S_LRC 25
#define I2S_DOUT 32

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  // Set volume (0 to 100)
  audio.setVolume(21);

  // Play an online radio stream
  audio.connecttohost("https://2.mystreaming.net:443/uber/crchopin/icecast.audio");
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  ESP_LOGI(TAG, "Entering Setup function");
  ESP_LOGI(TAG, "Heap initial size: %d bytes\n", ESP.getFreeHeap());
  ESP_LOGI(TAG, "Starting LCD init");

  LCD lcd;
  lcd.begin();

  ESP_LOGI(TAG, "Heap after LCD init: %d bytes\n", ESP.getFreeHeap());
  ESP_LOGI(TAG, "Starting WIFI init");

  String wifi_ip_address = wifi_init();
  if (wifi_ip_address.isEmpty())
  {
    ESP_LOGE(TAG, "Wifi init failed! Stopping...");
    lcd.print("No WiFi conn!");
    return;
  }

  ESP_LOGI(TAG, "Done WIFI init");
  ESP_LOGI(TAG, "Heap after wifi init: %d bytes\n", ESP.getFreeHeap());
  lcd.print("WiFi connected", wifi_ip_address.c_str());

  audio_init();
  ESP_LOGI(TAG, "Done audio init");
  ESP_LOGI(TAG, "Heap after audio init: %d bytes\n", ESP.getFreeHeap());
  lcd.print("Playing", "classics");
}

void loop()
{
  audio.loop();
}
