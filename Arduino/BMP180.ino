void logPressure() {
  // request temperature
  barometer.setControl(BMP085_MODE_TEMPERATURE);

  // wait appropriate time for conversion (4.5ms delay)
  unsigned long lastMicros = micros();
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

  // read calibrated temperature value in degrees Celsius
  float temperature = barometer.getTemperatureC();

  // request pressure (3x oversampling mode, high detail, 23.5ms delay)
  barometer.setControl(BMP085_MODE_PRESSURE_3);
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

  // read calibrated pressure value in Pascals (Pa)
  float pressure = barometer.getPressure()/100.0;


  // display measured values if appropriate
  logger.print("#PRS:");
  logger.print(millis());
  logger.print(",");
  logger.print(temperature); 
  logger.print(",");
  logger.print(pressure, 4);
  logger.println();

}

