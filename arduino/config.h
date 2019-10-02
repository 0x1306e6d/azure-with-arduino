#ifndef CONFIG_H_
#define CONFIG_H_

#include <DHT.h>

#define LED_BUILTIN 16

#define DHTPIN D4
#define DHTTYPE DHT11

#define IOTHUB_CONNECTION_STRING "HostName=<Your IoT Hub Name>.azure-devices.net;DeviceId=<Your Device Id>;SharedAccessKey=<Your Shared Access Key>"

#define IOT_CONFIG_WIFI_SSID "Your WiFi SSID"
#define IOT_CONFIG_WIFI_PASSWORD "Your WiFi Password"

#define NTP_SERVER "pool.ntp.org"
#define NTP_MIN_EPOCH (40 * 365 * 24 * 3600)

#endif
