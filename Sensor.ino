
#define BATTLEVEL
//#define DISPLAYEN
#define INTERVAL 1000


#include <SPI.h>
#ifdef DISPLAYEN
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#endif
#include <SD.h>
#include <TinyGPS.h>

#include "Arduino.h"
#include "Arduino.h"
#ifdef BATTLEVEL
#include "Wire.h"
#include "MAX1704.h"
#endif

#ifdef DISPLAYEN
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 7, 6);
#endif

#ifdef BATTLEVEL
MAX1704 fuelGauge;
#endif
TinyGPS gps;

int dustPin=2;
int dustVal=0;

int ledPower=8;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
unsigned long lastDebug;
unsigned long startMillis;

char filename[] = "d.txt";

float vRef = 1.1;

File logger;

void initSD();
File openFile();
void manageGPS();
void readLPGas();
void logLP();
int sampleSensor();
int sampleSensor100();

void setup(){
  Serial.begin(115200);
  Serial1.begin(9600);

#ifdef DISPLAYEN
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.display();
#endif

#ifdef BATTLEVEL
  fuelGauge.reset();
  fuelGauge.quickStart();
  fuelGauge.setAlertThreshold(10);
  fuelGauge.showConfig();
#endif


  pinMode(4,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(4, LOW);
  pinMode(ledPower,OUTPUT);
  analogReference(DEFAULT);
  digitalWrite(9, LOW);

  startMillis = millis();


  initSD();
  logger = openFile();

}


void loop(){
  manageGPS();
  readLPGas();

  if (millis()-lastDebug > INTERVAL) {
    lastDebug = millis();
    int sensorVal = sampleSensor100();
    float ppm = sensorVal*(2.56/1024)*0.172-0.0999;
    logLP();
    
    
    /*
    Serial.print("log: ");
    
    Serial.print(sensorVal);
    Serial.print("V");
    //analogWrite(9, sensorVal/4);

    
    
    Serial.print(" ppm: ");
    Serial.println(ppm, 4);*/


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
    int data;
    logger.close();

    logger = SD.open(filename);
    while ((data = logger.read()) >= 0) Serial.write(data);
    logger.close();


    if (Serial.read() == 'd') SD.remove(filename);

    logger = openFile();
  }


#ifdef DISPLAYEN
  displayData();
#endif

}





