#include <Arduino.h>
#include <WiFi.h>
#include <HttpClient.h>

#include "SparkFunLSM6DSO.h"
#include "Wire.h"

// network connection
char ssid[] = "Incognito Bob";    // your network SSID (name) 
char pass[] = "deeeeerp"; // your network password (use for WPA, or use as key for WEP)

// Name of the server we want to connect to
// const char kHostname[] = "worldtimeapi.org";
const char kHostname[] = "3.219.247.146";
const uint16_t kPort = 5000;
// Path to download (this is the bit after the hostname in the URL
// that you want to download
// const char kPath[] = "/api/timezone/Europe/London.txt";
const char kPath[] = "/?var=10";

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

// accelerometer
LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B


// DEBOUNCE STEPS
class Debouncer
{
    public:
      unsigned long lastActivation;
      int debounce_period;

      Debouncer(int d_period){
        lastActivation = millis();
        debounce_period = d_period;
      }

      bool activate(){
        if (millis() - lastActivation > debounce_period){
          lastActivation = millis();
          return true;
        }
        return false;
      }

      void changeDebouncePeriod(int d_period){
        debounce_period = d_period;
      }

};

void setup() {
  Serial.begin(9600);

  // start Accelerometer
  Wire.begin();
  delay(10);
  if( myIMU.begin() )
    Serial.println("Ready.");
  else { 
    Serial.println("Could not connect to IMU.");
    Serial.println("Freezing");
  }

  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");
}


#define step_threshold 0.3
#define step_debounce_time 750 // ms

int steps = 0;
Debouncer db(step_debounce_time);

void loop() {
  float x = myIMU.readFloatAccelX();
  float y = myIMU.readFloatAccelY();
  float z = myIMU.readFloatAccelZ();

  float step_acc = sqrtf(pow(x, 2) + pow(y, 2));
  

  if (step_acc > step_threshold && db.activate()){
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X = ");
    Serial.println(myIMU.readFloatAccelX(), 3);
    Serial.print(" Y = ");
    Serial.println(myIMU.readFloatAccelY(), 3);
    Serial.print(" Z = ");
    Serial.println(myIMU.readFloatAccelZ(), 3);

    Serial.print("Step acceleration: ");
    Serial.println(step_acc, 3);

    steps++;
    Serial.println("step taken");

    Serial.print("Steps taken: ");
	  Serial.println(steps);
  }
	
	//Display steps taken
	

  delay(100);
}