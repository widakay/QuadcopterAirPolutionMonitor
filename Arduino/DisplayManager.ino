#ifdef DISPLAYEN
void displayData() {
  display.clearDisplay();
#ifdef BATTLEVEL
  float charge = fuelGauge.stateOfCharge();
  float voltage = fuelGauge.batteryVoltage();

  display.print("Bat:");
  display.print(charge,0);
  display.print("%");
  display.print(" ");
  display.print(voltage,2);
  display.print("V");
  display.println();
#endif
  display.print("raw: ");
  display.print(sensorVal);
  display.println(" v");

  float ppm = sensorVal*0.172-0.0999;
  display.print("ppm: ");
  display.print(ppm, 4);
  display.println();

  float flat, flon;
  int year;

  byte month, day, hour, minute, second, hundredths;

  unsigned long age;

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  gps.f_get_position(&flat, &flon, &age);


  display.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 2);
  display.print(",");
  display.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 2);
  
  display.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
  display.print(" ");

  char sz[10];

  sprintf(sz, "%02d:%02d:%02d", hour, minute, second);

  display.println(sz);
  display.print(millis());
  
  display.display();

}
#endif




