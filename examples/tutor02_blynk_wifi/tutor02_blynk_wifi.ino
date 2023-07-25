#include "DINO_PLC.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

long lastTime,linkLastMill;
char auth[] = "tO8o9j1a2UsuJjf73f-Li4LP2Xny1dKJ";
char ssid[] = "JURAPORN_2.4G";
char pass[] = "044801060";
IPAddress lIP;

void updateLink(){
  if(millis() - linkLastMill > 1000){
    linkLastMill = millis();

    if(WiFi.status() == WL_CONNECTED){
      lIP = WiFi.localIP();
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
  Blynk.begin(auth,ssid,pass,"blynk.iot-cm.com", 8080);
}

void loop(){
  if(millis() - lastTime > 1000){    
    setU16D(100,random(0,9990));          //- Random value and assing to D100
    Blynk.virtualWrite(V0,getU16D(100));  //- Set to Blynk V0
    Blynk.virtualWrite(V1,getU16D(1));    //- Set to Blynk V1
  }
  Blynk.run();
  updateLink();
}
