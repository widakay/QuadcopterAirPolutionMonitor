
#ifdef DHTLOG
void setupDHT22() {
  dht.begin();
}

void logDHT22() {
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    //return;
  }
  logger.print("#DHT:");
  logger.print(millis());
  logger.print(",");
  logger.print(t);
  logger.print(",");
  logger.print(h);
  logger.println();
}

#endif
