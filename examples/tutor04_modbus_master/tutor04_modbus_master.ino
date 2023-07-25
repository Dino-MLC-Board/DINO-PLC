#include "DINO_PLC.h"
#include "ModbusClientRTU.h"

long lastTime;
ModbusClientRTU MB(Serial2);
bool data_ready = false;

#define NUM_VALUES 4
uint16_t values[NUM_VALUES];
float val[6];
uint32_t request_time;

void handleData(ModbusMessage response, uint32_t token){
  // First value is on pos 3, after server ID, function code and length byte
  uint16_t offs = 3;
  
  //- Token ID 1
  if(token == 1){
    for (uint8_t i = 0; i < 2; ++i) {
      offs = response.get(offs, values[i]);   //- Set data to D0
      setU16D(i,values[i]);
    }
  }

  //- Token ID 2
  if(token == 2){
    for (uint8_t i = 0; i < 4; ++i) {
      offs = response.get(offs, values[i]);   //- Set data from D10 to D10+4
      setU16D(10+i,values[i]);
    }
  }

  //- Token ID 3
  if(token == 3){
    for (uint8_t i = 0; i < 4; ++i) {
      offs = response.get(offs, values[i]);   //- Set data from D6 to D6+2   -- Float = 2 uint16
      setU16D(4+i,values[i]);
    }
  }
}

void handleError(Error error, uint32_t token){
  // ModbusError wraps the error code and provides a readable error message for it
  ModbusError me(error);
  Serial.printf("Error response: %02X - %s\n", (int)me, (const char *)me);
}

void setup(){
  initPLC();
  //- eModbus Configurations
  Serial2.begin(9600,SERIAL_8N1,16,17);
  MB.onDataHandler(&handleData);            //- Config handle data
  MB.onErrorHandler(&handleError);          //- Config handle error 
  MB.setTimeout(2000);
  MB.begin();
}

void loop(){
  if(millis() - lastTime > 1000){
    lastTime = millis();

    MB.addRequest(1, 1, READ_INPUT_REGISTER, 1, 2);    //- 1 = token id ,1 = slave id ,read input register ,data addr , count
    //MB.addRequest(2, 1, READ_HOLD_REGISTER, 0, 4);     //- 2 = token id ,1 = slave id ,read holding register ,data addr , count
    //MB.addRequest(3, 1, READ_INPUT_REGISTER, 4, 4);    //- 3 = token id ,1 = slave id ,read input register ,data addr , count
    /*
    Error err = MB.addRequest((uint32_t)millis(), 1, READ_INPUT_REGISTER, 0, NUM_VALUES);
    if (err!=SUCCESS) {
      ModbusError e(err);
      Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    }
    */
  }
}
