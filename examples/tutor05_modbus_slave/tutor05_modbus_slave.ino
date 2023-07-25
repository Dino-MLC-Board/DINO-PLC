#include "DINO_PLC.h"
#include "ModbusServerRTU.h"

long lastTime;
#define MAX_RTU     30
#define MAX_ADDR    30
ModbusServerRTU mbRTU(Serial2,2000);

ModbusMessage FC03(ModbusMessage request) {
  uint16_t address;           // requested register address
  uint16_t words;             // requested number of registers
  ModbusMessage response;     // response message to be sent back

  // get request values
  request.get(2, address);
  request.get(4, words);

  // Address and words valid? --> MAX_RTU
  if(address >= 0 && words && (address + words) <= MAX_RTU){
    // Looks okay. Set up message with serverID, FC and length of data
    response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));
    // Fill response with requested data
    for(uint16_t i = address; i < address + words; ++i){
      response.add(getU16D(i));
    }
  }else{
    // No, either address or words are outside the limits. Set up error response.
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
  }
  return response;
}

ModbusMessage FC06(ModbusMessage request) {
  uint16_t addr = 0;        // Start address to read
  uint16_t value = 0;       // New value for register
  ModbusMessage response;

  // Get addr and value from data array. Values are MSB-first, getValue() will convert to binary
  request.get(2, addr);
  request.get(4, value);

  // address valid?
  if (addr < 0 || addr > MAX_ADDR){
    // No. Return error response
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    return response;
  }

  // Modbus address is 1..n, memory address 0..n-1
  //addr--;

  // Fake data error - 0x0000 or 0xFFFF will not be accepted
  if (!value || value == 0xFFFF) {
    // No. Return error response
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_VALUE);
    return response;
  }

  // Fill in new value.
  setU16D(addr,value);

  // Return the ECHO response
  return ECHO_RESPONSE;
}

void setup(){
  initPLC();

  //- Serial 2
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  mbRTU.registerWorker(1, READ_HOLD_REGISTER, &FC03);    //- Slave ID ,Read holding register , FC handle
  mbRTU.registerWorker(1, WRITE_HOLD_REGISTER, &FC06);   //- Slave ID ,Read holding register , FC handle
  mbRTU.start();
}

void loop(){
  if(millis() - lastTime > 1000){
    lastTime = millis();

  }
}
