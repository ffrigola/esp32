#include <Arduino.h>
#include "EmonLib.h"
#include "WiFi.h"
#include <driver/adc.h>
#include "config/config.h"
#include "config/enums.h"
#include "config/topic-aws.h"

#include "tasks/mqtt-aws.h"
#include "tasks/wifi-connection.h"
#include "tasks/measure-electricity.h"

#include "string.h"

EnergyMonitor emon1;

// Place to store local measurements before sending them off to AWS
float measurements[LOCAL_MEASUREMENTS];
unsigned int measureIndex = 0;
String string_amps;

void setup()
{
  #if DEBUG == true
    Serial.begin(115200);
  #endif 

  // Setup the ADC
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);
  pinMode(ADC_INPUT, INPUT);


  // Initialize emon library
  emon1.current(ADC_INPUT, 30);

  // ----------------------------------------------------------------
  // TASK: Connect to WiFi & keep the connection alive.
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
    keepWiFiAlive,
    "keepWiFiAlive",  // Task name
    5000,            // Stack size (bytes)
    NULL,             // Parameter
    1,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );

  // ----------------------------------------------------------------
  // TASK: Connect to AWS & keep the connection alive.
  // ----------------------------------------------------------------
  #if AWS_ENABLED == true
    xTaskCreate(
      keepAWSConnectionAlive,
      "MQTT-AWS",      // Task name
      5000,            // Stack size (bytes)
      NULL,             // Parameter
      5,                // Task priority
      NULL              // Task handle
    );
  #endif


  // ----------------------------------------------------------------
  // Task: measure electricity consumption ;)
  // ----------------------------------------------------------------
  xTaskCreate(
    measureElectricity,
    "Measure electricity",  // Task name
    5000,                  // Stack size (bytes)
    NULL,                   // Parameter
    4,                      // Task priority
    NULL                    // Task handle
  );
  
  // ----------------------------------------------------------------
  // Define the topic name
  // ----------------------------------------------------------------
  
    AWS_IOT_TOPIC = GetTopic(DeviceID);


  #if HA_ENABLED == true
    xTaskCreate(
      HADiscovery,
      "MQTT-HA Discovery",  // Task name
      5000,                // Stack size (bytes)
      NULL,                 // Parameter
      5,                    // Task priority
      NULL                  // Task handle
    );

    xTaskCreate(
      keepHAConnectionAlive,
      "MQTT-HA Connect",
      5000,
      NULL,
      4,
      NULL
    );
  #endif
}

void loop()
{
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}