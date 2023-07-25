#include <DINO_PLC.h>
extern bool USE_PLC_MODBUS;           //- Extern configuration variable from internal PLC register

void setup(){
  USE_PLC_MODBUS = true;
  initPLC();
}

void loop(){

}