#ifndef CONFIG
#define CONFIG

/**
 * Set this to false to disable Serial logging
 */
#define DEBUG true

/**
 * The name of this device (as defined in the AWS IOT console).
 * Also used to set the hostname on the network
 */
#define DEVICE_NAME "esp32-ffff3f40"

/**
 * ADC input pin that is used to read out the CT sensor
 */
#define ADC_INPUT 39

/**
 * WiFi credentials
 */
#define WIFI_NETWORK "MOOMA"
#define WIFI_PASSWORD "sidramooma"

/**
 * Timeout for the WiFi connection. When this is reached,
 * the ESP goes into deep sleep for 30seconds to try and
 * recover.
 */
#define WIFI_TIMEOUT 20000 // 20 seconds

/**
 * How long should we wait after a failed WiFi connection
 * before trying to set one up again.
 */
#define WIFI_RECOVER_TIME_MS 20000 // 20 seconds


/**
 * Force Emonlib to assume a 3.3V supply to the CT sensor
 */
#define emonTxV3 1


/**
 * Local measurements
 */
#define LOCAL_MEASUREMENTS 5


/**
 * The MQTT endpoint of the service we should connect to and receive messages
 * from.
 */


#define AWS_ENABLED true
#define AWS_IOT_ENDPOINT "a34p2rq5aahzw3-ats.iot.eu-west-1.amazonaws.com"

/* The topic name defined as a String and the last 4 numbers of the DeviceID
*/

String AWS_IOT_TOPIC;

uint64_t chipid = ESP.getEfuseMac();
int16_t chip = (uint16_t)(chipid >> 32); // Last 4 values of MAC address
String DeviceID = String(chip, HEX);



#define MQTT_CONNECT_DELAY 200
#define MQTT_CONNECT_TIMEOUT 20000 // 20 seconds


// Check which core Arduino is running on. This is done because updating the 
// display only works from the Arduino core.
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#endif