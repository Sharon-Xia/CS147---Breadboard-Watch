#include <Arduino.h>
#include <WiFi.h>
#include <HttpClient.h>

#include "SparkFunLSM6DSO.h"
#include "Wire.h"

#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>

// TODO: Display steps on ESP32 display


// *** NETWORK CONNECTION
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


// BPM RECORD
class HeartRateDocumentor {
  public:

    unsigned long lastBeat;

    HeartRateDocumentor(){
      lastBeat = millis();
    }

    void newBeat(){

    }
};

// HEART RATE SENSOR

const int PULSE_INPUT = A0;
const int PULSE_BLINK = 32;    // Pin 13 is the on-board LED
const int PULSE_FADE = 5;
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;

const int OUTPUT_TYPE = SERIAL_PLOTTER;

void setupHeartSensor(){
  // Configure the PulseSensor manager.
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  // Skip the first SAMPLES_PER_SERIAL_SAMPLE in the loop().
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

  // Now that everything is ready, start reading the PulseSensor signal.
  if (!pulseSensor.begin()) {
    /*
       PulseSensor initialization failed,
       likely because our Arduino platform interrupts
       aren't supported yet.

       If your Sketch hangs here, try changing USE_PS_INTERRUPT to false.
    */
    for(;;) {
      // Flash the led to show things didn't work.
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

void setupAccelerometer(){
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

void setupWifi(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  delay(10);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());
}

void setup() {
  Serial.begin(115200);

  setupHeartSensor();

  setupAccelerometer();

  setupWifi();
}


HeartRateDocumentor heartRateDocumentor;

int readPulse(){
  int bpm = -1;
  if (pulseSensor.sawNewSample()) {
    /*
       Every so often, send the latest Sample.
       We don't print every sample, because our baud rate
       won't support that much I/O.
    */
    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

      //pulseSensor.outputSample();

      /*
         At about the beginning of every heartbeat,
         report the heart rate and inter-beat-interval.
      */
      if (pulseSensor.sawStartOfBeat()) {
        pulseSensor.outputBeat();
        bpm = pulseSensor.getBeatsPerMinute();
        Serial.println("bpm: " + String(bpm));
        heartRateDocumentor.newBeat();
      }
    }

    /*******
      Here is a good place to add code that could take up
      to a millisecond or so to run.
    *******/
  }
  return bpm;
}


#define step_threshold 1.2
#define step_debounce_time 750 // ms

int steps = 0;
Debouncer db(step_debounce_time);

unsigned long lastCount = millis();

int stepCounter(){
  // check steps every 100 ms
  if (millis() > lastCount + 100){
    lastCount = millis();
  } else return 0;

  float x = myIMU.readFloatAccelX();
  float y = myIMU.readFloatAccelY();
  float z = myIMU.readFloatAccelZ();

  float step_acc = pow(pow(x, 2) + pow(y, 2) + pow(z, 2), 0.33333333);  

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
    return 1;
  }
  return 0;
	
	//Display steps taken
}


char *remove_white_spaces(char *str)
{
	int i = 0, j = 0;
	while (str[i])
	{
		if (str[i] != ' ')
          str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	return str;
}

void sendDataToServer(int bpm, int steps){
  int err =0;
  
  WiFiClient c;
  HttpClient http(c);

  char bpmStr[16];
  char stepsStr[16];

  itoa(bpm, bpmStr, 10);
  itoa(steps, stepsStr, 10);

  remove_white_spaces(bpmStr);
  remove_white_spaces(stepsStr);


  char sensorKPath[26] = "/?bpm=";
  strcat(sensorKPath, bpmStr);
  strcat(sensorKPath, "&steps=");
  strcat(sensorKPath, stepsStr);

  Serial.println(sensorKPath);

  delay(20);
  
  err = http.get(kHostname, kPort, sensorKPath);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
               
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  // And just stop, now that we've tried a download
  //while(1);
  delay(100);
}

void loop() {

  int bpm = readPulse();
  
  int steps = stepCounter();

  if (!(bpm==-1 && steps==0))
    sendDataToServer(bpm, steps);
}