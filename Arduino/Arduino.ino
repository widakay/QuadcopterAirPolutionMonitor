
#define BATTLEVEL
#define DISPLAYEN
#define BMP180
//#define SDLog

#define INTERVAL 1000


#include <SPI.h>
#ifdef DISPLAYEN
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#endif
#include <SD.h>
#include <TinyGPS.h>

#include "Arduino.h"
#ifdef BATTLEVEL
#include "Wire.h"
#include "MAX1704.h"
#endif

#ifdef BMP180
#include "I2Cdev.h"
#include "BMP085.h"
BMP085 barometer;
#endif

#ifdef DISPLAYEN
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 5, 9);
#endif

#ifdef BATTLEVEL
MAX1704 fuelGauge;
#endif
TinyGPS gps;

int dustPin=2; // analog input pin 2
int ledPower=14;
int LPGasPin = 2; // digital io pin 2

int delayTime=280;
int delayTime2=40;
float offTime=9680;
float sensorVal = 0;

float soc[120];
int socCount = 0;


unsigned long lastDebug;
unsigned long startMillis;

char filename[] = "d.txt";

float vRef = 1.1;

#ifdef SDLog
File logger;
#else
#define logger Serial
#endif

void setup(){
  Serial.begin(115200);
  Serial1.begin(9600);

#ifdef DISPLAYEN
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.print("Running!");
  display.display();
#endif

  delay(1000);
  Serial.println("Running");

#ifdef BATTLEVEL
  Wire.begin();
  fuelGauge.reset();
  fuelGauge.quickStart();
  fuelGauge.setAlertThreshold(10);
  fuelGauge.showConfig();
#endif

#ifdef BMP180
  barometer.initialize();
  Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");
#endif


  pinMode(ledPower,OUTPUT);
  pinMode(LPGasPin,OUTPUT);
  digitalWrite(ledPower, LOW);
  //analogReference(DEFAULT);
  digitalWrite(LPGasPin, LOW);

  startMillis = millis();

#ifdef SDLog
  initSD();
  logger = openFile();
#endif
}


void loop() {
  manageGPS();
  updateLPGas();

  if (millis()-lastDebug > INTERVAL) {

#ifdef DISPLAYEN
    displayData();
#endif
    lastDebug = millis();
    #ifdef BMP180
    logPressure();
    #endif
    float sensorVal = sampleSensor100();
    float ppm = sensorVal*(3.3/1024)*0.172-0.0999;
    logLP();


    logger.print("#PPM:");
    logger.print(millis());
    logger.print(",");
    logger.print(sensorVal);
    logger.print(",");
    logger.print(ppm, 4);
    logger.println();
    logger.flush();
#ifdef BATTLEVEL
    float charge = fuelGauge.stateOfCharge();
    float voltage = fuelGauge.batteryVoltage();
    /*
    Serial.print(",batt:");
     Serial.print(charge,2);
     Serial.print("%,");
     Serial.print(voltage,4);
     Serial.println("V");*/

    logger.print("#BATT:");
    logger.print(millis());
    logger.print(",");
    logger.print(charge,2);
    logger.print("");
    logger.print(",");
    logger.print(voltage,4);
    logger.println();
    logger.flush();
#endif
  }
  if (Serial.available()) {
#ifdef SDLog

    int data;
    logger.close();

    if (Serial.read() == 'd') {
      SD.remove(filename);
    }
    else {

      logger = SD.open(filename);
      while ((data = logger.read()) >= 0) Serial.write(data);
      logger.close();
    }
    logger = openFile();
#endif
  }



}















