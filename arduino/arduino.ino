#include "config.h"

#include <time.h>

#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <ESP8266WiFi.h>

void initSerial()
{
  // Start serial and initialize stdout
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Serial.begin OK");
}

void initWifi()
{
  // Attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(IOT_CONFIG_WIFI_SSID);

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(IOT_CONFIG_WIFI_SSID, IOT_CONFIG_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi.begin OK");
}

void initTime()
{
  time_t epochTime;

  configTime(0, 0, NTP_SERVER);

  while (true)
  {
    epochTime = time(NULL);

    if (epochTime < NTP_MIN_EPOCH)
    {
      Serial.println("configTime FAIL");
      delay(2000);
    }
    else
    {
      Serial.print("configTime OK, epochTime: ");
      Serial.println(epochTime);
      break;
    }
  }
}

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle = NULL;

void initIoTHubClient()
{
  iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(IOTHUB_CONNECTION_STRING, MQTT_Protocol);
  if (iotHubClientHandle != NULL)
  {
    Serial.println("IoTHubClient_LL_CreateFromConnectionString OK");
  }
  else
  {
    Serial.println("IoTHubClient_LL_CreateFromConnectionString FAIL");
  }
}

// the setup function runs once when you press reset or power the board
void setup()
{
  initSerial();

  initWifi();
  initTime();

  initIoTHubClient();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
  if (iotHubClientHandle != NULL)
  {
    IoTHubClient_LL_DoWork(iotHubClientHandle);
  }

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
