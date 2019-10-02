#include "config.h"

#include <time.h>

#include <ArduinoJson.h>
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
static unsigned int iotHubMessageTrackingId = 0;

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

void sendIoTHubMessageCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
  unsigned int messageTrackingId = (unsigned int)(uintptr_t)userContextCallback;

  digitalWrite(LED_BUILTIN, HIGH); // turn the LED off

  Serial.print("sendIoTHubMessageCallback ");
  Serial.print(ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
  Serial.print(", messageTrackingId: ");
  Serial.println(messageTrackingId);
}

void sendIoTHubMessage(const char *source)
{
  IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromString(source);
  if (messageHandle == NULL)
  {
    Serial.println("IoTHubMessage_CreateFromString FAIL");
  }
  else
  {
    if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, sendIoTHubMessageCallback, (void *)(uintptr_t)iotHubMessageTrackingId) == IOTHUB_CLIENT_OK)
    {
      digitalWrite(LED_BUILTIN, LOW); // turn the LED on

      Serial.println("IoTHubClient_LL_SendEventAsync OK");
    }
    else
    {
      Serial.println("IoTHubClient_LL_SendEventAsync FAIL");
    }

    IoTHubMessage_Destroy(messageHandle);
  }

  iotHubMessageTrackingId++;
}

static DHT dht(DHTPIN, DHTTYPE);

void initDHT()
{
  dht.begin();
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
static int loopCount = 0;
void loop()
{
  if (iotHubClientHandle != NULL)
  {
    if ((loopCount % 1000) == 0)
    {
      StaticJsonDocument<256> json;
      json["temperature"] = 0;
      json["humidity"] = 0;

      char buffer[256];
      serializeJson(json, buffer, 256);
      sendIoTHubMessage(buffer);
    }

    IoTHubClient_LL_DoWork(iotHubClientHandle);
    loopCount++;
  }

  delay(100);
}
