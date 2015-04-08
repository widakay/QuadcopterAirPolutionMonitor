void initSD() {
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(10)) {
    // don't do anything more:
    Serial.println("failed to init SD...");
  }
}
  
File openFile() {
  File dataFile = SD.open("d.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    return dataFile;
  }
}


