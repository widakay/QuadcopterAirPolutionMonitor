
void manageGPS() {

  bool newData = false;
  while (Serial1.available())
  {
    char c = Serial1.read();
    //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
    if (gps.encode(c)) // Did a new valid sentence come in?
      newData = true;
  }
  if (newData) {
    float flat, flon;
    unsigned long age, date, time;


    //gps.get_datetime(&date, &time, &age);

    gps.f_get_position(&flat, &flon, &age);
    logger.print("#GPS:");
    logger.print(millis());
    logger.print(",");
    logger.print(date);
    logger.print(",");
    logger.print(time);
    logger.print(",");
    logger.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    logger.print(",");
    logger.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    logger.print(",");
    logger.print(gps.f_altitude());
    logger.print(",");
    logger.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    logger.print(",");
    logger.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    logger.println();
    logger.flush();
  }
  /*
  
   unsigned long chars;
   unsigned short sentences, failed;
   gps.stats(&chars, &sentences, &failed);
   logger.println("\n");
   logger.print(" CHARS=");
   logger.print(chars);
   logger.print(" SENTENCES=");
   logger.print(sentences);
   logger.print(" CSUM ERR=");
   logger.println(failed);
   logger.println();*/
}



