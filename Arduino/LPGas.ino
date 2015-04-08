#define MQ6_POWER_ON_DIGITAL_OUT_PIN 2
#define MQ6_LP_GAS_LEVEL_MEASURE_ANALOG_IN_PIN   1
#define MQ6_HEATER_TIME_MILLIS 90000
#define MQ6_SAMPLE_PERIOD_MILLIS 1000

int lpGas;

typedef enum {
  ST_MQ6_OFF,
  ST_MQ6_CYCLE_0_HIGH,
  ST_MQ6_DONE
} 
MQ6_STATE;

MQ6_STATE mq6State = ST_MQ6_OFF;

unsigned long mq6SwitchTimeMillis;
unsigned long mq6NextReadingTimeMillis;
unsigned long lpGasStartTimeMillis = 10000;   // start 10s after powerup


void logLP() {
  lpGas = analogRead(MQ6_LP_GAS_LEVEL_MEASURE_ANALOG_IN_PIN);

  logger.print("#LP:");
  logger.print(millis());
  logger.print(",");
  logger.print(digitalRead(MQ6_POWER_ON_DIGITAL_OUT_PIN));
  logger.print(",");
  logger.print(lpGas);
  logger.println();
  logger.flush();
/*
  Serial.print(",LP:");
  Serial.print(lpGas);*/
}



//-----------------------------------------------------
// uses a simple finite state machine to manage states of the MQ6 sensor
void readLPGas(){
  switch(mq6State){
  case ST_MQ6_OFF :
    {
      if(millis() > lpGasStartTimeMillis){
        digitalWrite(MQ6_POWER_ON_DIGITAL_OUT_PIN, HIGH);

        mq6State = ST_MQ6_CYCLE_0_HIGH;
        mq6SwitchTimeMillis = millis() + MQ6_HEATER_TIME_MILLIS;

        Serial.print("H");

      }
      break;
    }

  case ST_MQ6_CYCLE_0_HIGH :
    {
      if(millis() > mq6NextReadingTimeMillis) {
        lpGas = analogRead(MQ6_LP_GAS_LEVEL_MEASURE_ANALOG_IN_PIN);


        mq6NextReadingTimeMillis = millis() + MQ6_SAMPLE_PERIOD_MILLIS;
      }

      if(millis() > mq6SwitchTimeMillis){
        digitalWrite(MQ6_POWER_ON_DIGITAL_OUT_PIN, LOW);
        Serial.print("C");
        mq6State = ST_MQ6_DONE;
      }

      break;
    }

  case ST_MQ6_DONE :
    {
      lpGasStartTimeMillis = millis() + 30000;
      mq6State = ST_MQ6_OFF;
      break;
    }
  }
}


