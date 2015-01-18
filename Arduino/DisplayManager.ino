#ifdef DISPLAYEN
void displayData() {
  display.clearDisplay();
  
  #ifdef BATTLEVEL
  float charge = fuelGauge.stateOfCharge();
  display.println("Bat: ");
  display.print(charge,0);
  display.print("%");
  display.print(" ");
  

  float voltage = fuelGauge.batteryVoltage();
  display.print(voltage,4);
  display.println("V");
  #endif
  
  display.setTextSize(2);
  display.println("Dust:");

  display.setTextSize(1);
  display.print("raw: ");
  float sensorVal = sampleSensor100()*(vRef/1024);
  display.print(sensorVal);
  display.println(" v");

  float ppm = sensorVal*0.172-0.0999;
  display.print("ppm: ");
  display.print(ppm, 4);
  display.println();
  display.display();

}
#endif
