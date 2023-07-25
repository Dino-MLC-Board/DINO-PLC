#include <SPI.h>
#include <Ethernet.h>
#include <DINO_PLC.h>
#include "ModbusServerEthernet.h"

/*
ต้อง Modify C:\Users\super\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.9\cores\esp32\Server.h
virtual void begin(uint16_t port=0) =0; >>> virtual void begin() =0;
*/

ModbusServerEthernet mTCPServer;

//---------------- Ethernet W5500
#define ETH_RST   22
#define ETH_CS    5
#define ETH_SCLK  18
#define ETH_MISO  23
#define ETH_MOSI  19
#define NET       33

#define MAX_DATA  200

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
bool ethernet_connected;
long ethernet_lastmill;
IPAddress lIP;
long linkLastMill;

void ethernetReset(const uint8_t resetPin){
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  delay(250);
  digitalWrite(resetPin, LOW);
  delay(50);
  digitalWrite(resetPin, HIGH);
  delay(350);
}

void initEthernet(){
  SPI.begin(ETH_SCLK,ETH_MISO,ETH_MOSI,ETH_CS);
  ethernetReset(ETH_RST);
  Ethernet.init(ETH_CS);
  Ethernet.begin(mac);
  pinMode(NET,OUTPUT);
  delay(2000);
}

void updateLink(){
  if(millis() - linkLastMill > 1000){
    linkLastMill = millis();
    auto link = Ethernet.linkStatus();
    switch(link){
      case Unknown:
        //Serial.println("Unknown");
        digitalWrite(NET,HIGH);
        break;
      case LinkON:
        //Serial.println("ON");
        digitalWrite(NET,LOW);
        break;
      case LinkOFF:
        //Serial.println("OFF");
        digitalWrite(NET,HIGH);
        break;
    }
    if(Ethernet.linkStatus() == LinkON){
      lIP = Ethernet.localIP();
      setU16D(700,lIP[0]); setU16D(701,lIP[1]); setU16D(702,lIP[2]); setU16D(703,lIP[3]);
    }else{
      setU16D(700,0); setU16D(701,0); setU16D(702,0); setU16D(703,0);
    }
  }
}

//---------------- Modbus TCP server
ModbusMessage FC03(ModbusMessage request){
  ModbusMessage response; // The Modbus message we are going to give back
  uint16_t addr = 0;      // Start address
  uint16_t words = 0;     // # of words requested
  request.get(2, addr);   // read address from request
  request.get(4, words);  // read # of words from request

  // Address overflow?
  if((addr + words) > MAX_DATA){
    // Yes - send respective error response
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
  }
  // Set up response
  response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));
  // Request for FC 0x03?
  if(request.getFunctionCode() == READ_HOLD_REGISTER){
    // Internal CPU temperature >> ADDR 0
    for(uint8_t i = 0; i < words; ++i){
      //- Get D0 - Dxxx
      response.add(getU16D(i));
    }
  }else{
    // No, this is for FC 0x04. Response is random
    for(uint8_t i = 0; i < words; ++i){
      // send increasing data values
      response.add((uint16_t)random(1, 65535));
    }
  }
  // Send response back
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
  if (addr < 0 || addr > MAX_DATA) {
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
  //memo[addr] = value;
  setU16D(addr,value);

  // Return the ECHO response
  return ECHO_RESPONSE;
}

//---------------- Main loop
void setup(){
  initPLC();
  initEthernet();
  Serial2.begin(9600,SERIAL_8N1,16,17);

  mTCPServer.registerWorker(1, READ_HOLD_REGISTER, &FC03);      // FC=03 for serverID=1
  mTCPServer.registerWorker(1, READ_INPUT_REGISTER, &FC03);     // FC=04 for serverID=1
  mTCPServer.registerWorker(1, WRITE_HOLD_REGISTER, &FC06);     // FC=06 for serverID=1
  mTCPServer.start(502,4,20000);

}

void loop(){
  updateLink();
}