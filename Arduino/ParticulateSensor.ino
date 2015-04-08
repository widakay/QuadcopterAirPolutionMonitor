
float sampleSensor100() {

  //analogReference(INTERNAL);
  long sum = 0;
  for (int i=0;i<25;i++) {
    sum += sampleSensor();
  }
  //analogReference(DEFAULT);
  sensorVal = sum/25.0;
  sensorVal *= (3.3/1024);
  return sensorVal;
}

int sampleSensor() {
  digitalWrite(ledPower,HIGH); // power on the LED
  delayMicroseconds(delayTime);
  int dustVal=analogRead(dustPin); // read the dust value via pin 5 on the sensor
  delayMicroseconds(delayTime2);
  digitalWrite(ledPower,LOW); // turn the LED off
  delayMicroseconds(offTime);
  return dustVal;
}



