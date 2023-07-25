#include "DINO_PLC.h"
#include <SPI.h>
#include <Ethernet.h>

//#define BLYNK_PRINT Serial
#include <BlynkSimpleEthernet.h>

//---------------- Ethernet W5500
#define ETH_RST   22
#define ETH_CS    5
#define ETH_SCLK  18
#define ETH_MISO  23
#define ETH_MOSI  19
#define NET       33

#define W5100_CS  ETH_CS

long lastTime;
char auth[] = "tO8o9j1a2UsuJjf73f-Li4LP2Xny1dKJ";
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
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
  if(Ethernet.linkStatus() == LinkON){
    lIP = Ethernet.localIP();

    //Serial.printf("My IP address: %u.%u.%u.%u\n", lIP[0], lIP[1], lIP[2], lIP[3]);
  }
}

void updateLink(){
  if(millis() - linkLastMill > 1000){
    linkLastMill = millis();

    auto link = Ethernet.linkStatus();
    switch (link) {
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

BLYNK_WRITE(V10){                           //- Received v10 data to be used to control M100
  bool v10_cmd  = param.asInt();
  if(v10_cmd){
    setM(2);
  }else{
    resetM(2);
  }
}
BLYNK_WRITE(V2){                           //- Received v10 data to be used to control D4
  uint16_t v2_cmd  = param.asInt();
  setU16D(2,v2_cmd);
}

void setup(){
  initPLC();
  //- SPI Ethernet object
  SPI.begin(ETH_SCLK,ETH_MISO,ETH_MOSI,ETH_CS);
  ethernetReset(ETH_RST);
  initEthernet();
  //- Connect to Blynk v1.0
  Blynk.begin(auth,"blynk.iot-cm.com", 8080);
}

void loop(){
  if(millis() - lastTime > 1000){    
    setU16D(100,random(0,9990));          //- Random value and assing to D100
    Blynk.virtualWrite(V0,getU16D(100));  //- Set to Blynk V0
    Blynk.virtualWrite(V1,getU16D(1));    //- Set to Blynk V1
  }
  updateLink();                           //- Update NET LED status
  Blynk.run();
}
