#include <ESP32Time.h>
#include <DINO_PLC.h>

ESP32Time ertc(3600*7);
long lastMill;

void setup(){
  initPLC();
  ertc.setTime(30, 24, 15, 17, 7, 2021);

  //xTaskCreatePinnedToCore(RTC0_Task,"RTC0_Task",1000,(void*) NULL,1,&t100,1);
}

void loop(){
  if(millis() - lastMill > 1000){
    lastMill = millis();
    //struct tm timeinfo = ertc.getTimeStruct();
    //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");   //  (tm struct) Sunday, January 17 2021 07:24:38		
    setmyRTCTime(ertc.getSecond(),ertc.getMinute(),ertc.getHour(),ertc.getDay(),ertc.getMonth(),ertc.getYear(),0);
  }
}