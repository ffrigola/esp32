#ifndef TASK_MEASURE_ELECTRICITY
#define TASK_MEASURE_ELECTRICITY

#include <Arduino.h>
#include "EmonLib.h"

#include "../config/config.h"
#include "../config/enums.h"
#include "../config/topic-aws.h"
#include "mqtt-aws.h"

#include <iostream>
#include <string.h>


extern EnergyMonitor emon1;
extern float measurements[LOCAL_MEASUREMENTS];
extern unsigned int measureIndex;

float amps;
float sum_amps;
float mean_amps;
char char_amps[10];
extern String string_amps;

void measureElectricity(void * parameter)
{
    for(;;){
      serial_println("[ENERGY] Measuring...");
      long start = millis();

      double amps = emon1.calcIrms(1480);

      measurements[measureIndex] = amps;
      measureIndex++;

      if(measureIndex == LOCAL_MEASUREMENTS){

        for(int i=0; i < int(LOCAL_MEASUREMENTS); i++){
           sum_amps += measurements[i];
        }

        mean_amps = sum_amps/float(LOCAL_MEASUREMENTS);
        sprintf(char_amps,"%f",mean_amps);
        string_amps = String(char_amps);

        if( mean_amps < 0.5){
          string_amps = "0";
        }
        serial_println(measurements[0]);
        serial_println(measurements[1]);
        serial_println(measurements[2]);
        serial_println(measurements[3]);
        serial_println(measurements[4]);
        serial_println(sum_amps);
        serial_println(mean_amps);
       
          #if AWS_ENABLED == true
            xTaskCreate(
              uploadMeasurementsToAWS,
              "Upload measurements to AWS",
              10000,             // Stack size (bytes)
              NULL,             // Parameter
              5,                // Task priority
              NULL              // Task handle
            );
          #endif

          #if HA_ENABLED == true
            xTaskCreate(
              sendEnergyToHA,
              "HA-MQTT Upload",
              10000,             // Stack size (bytes)
              NULL,             // Parameter
              5,                // Task priority
              NULL              // Task handle
            );
          #endif

          measureIndex = 0;
          sum_amps = 0;
      }

      long end = millis();

      // Schedule the task to run again in 12 seconds (while
      // taking into account how long measurement took)
      vTaskDelay((12000-(end-start)) / portTICK_PERIOD_MS);
    }    
}

#endif