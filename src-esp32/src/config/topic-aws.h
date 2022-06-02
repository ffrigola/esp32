#ifndef topic_aws
#define topic_aws
#include <Arduino.h>
#include "stdio.h"
#include "string.h"

#include "../config/config.h"
#include "../tasks/measure-electricity.h"

 String GetTopic (String DeviceID){
  String AWS_IOT_TOPIC;
  AWS_IOT_TOPIC = "sensors/power/"+ DeviceID;
  return AWS_IOT_TOPIC;
  } 

#endif
