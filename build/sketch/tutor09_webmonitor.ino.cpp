#include <Arduino.h>
#line 1 "d:\\Project\\K.Saravut\\git\\DINO-PLC\\DINO-PLC\\examples\\tutor09_webmonitor\\tutor09_webmonitor.ino"
#include <DINO_PLC.h>

extern bool USE_PLC_MODBUS;
extern bool USE_PLC_RTC;
extern int  RTC_SYNC_DURATION;
extern bool USE_PLC_MODBUS_TCP;

#line 8 "d:\\Project\\K.Saravut\\git\\DINO-PLC\\DINO-PLC\\examples\\tutor09_webmonitor\\tutor09_webmonitor.ino"
void setup();
#line 17 "d:\\Project\\K.Saravut\\git\\DINO-PLC\\DINO-PLC\\examples\\tutor09_webmonitor\\tutor09_webmonitor.ino"
void loop();
#line 8 "d:\\Project\\K.Saravut\\git\\DINO-PLC\\DINO-PLC\\examples\\tutor09_webmonitor\\tutor09_webmonitor.ino"
void setup(){
  USE_PLC_MODBUS      =  true;                  //- Use PLC Modbus function
  USE_PLC_RTC         =  true;                  //- Use PLC RTC function
  RTC_SYNC_DURATION   =  60;                    //- RTC Sync time (Min.)
  USE_PLC_MODBUS_TCP  =  true;                  //- Enable Modbus TCP Server
  initPLC();                                    //- Init FX1N PLC function

}

void loop(){
  PLC_Network_Function();                     //- Very important must be in main loop and no Delay in main loop
}
