#ifndef DINO_PLC_H
#define DINO_PLC_H
#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <ESP32Time.h>
#include <WiFi.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PubSubClient.h>
#define  NO_GLOBAL_BLYNK
#include <BlynkSimpleEthernet.h>
#include <ModbusServerEthernet.h>

//--------------------------- Main Function Parts
void     initPLC(uint16_t boardver = 100);
bool     getM(uint16_t addr);
void     setM(uint16_t addr);
void     resetM(uint16_t addr);
uint8_t  getU8D(uint16_t addr);
uint16_t getU16D(uint16_t addr);
uint32_t getU32D(uint16_t addr);
uint16_t getT(uint16_t _t);
uint16_t getC(uint16_t _t);
float    getFD(uint16_t addr);
void     setU8D(uint16_t addr,uint8_t val);
void     setU16D(uint16_t addr,uint16_t val);
void     setU32D(uint16_t addr,uint32_t val);
void     setFD(uint16_t addr,float val);

void     initRTC();
void     RTCSettime(int sc,int min,int hr,int da,int mt,int yr);
void     updateRTC();
void     setmyRTCTime(uint16_t s,uint16_t m,uint16_t h,uint16_t d,uint16_t mo,uint16_t yr,uint16_t dw);

void     PLC_Network_Function();

#endif