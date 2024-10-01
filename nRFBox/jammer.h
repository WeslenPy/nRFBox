/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */
   
#ifndef jammer_H
#define jammer_H

#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <RF24.h>
#include "ESP8266WiFi.h"

void jammerSetup();
void jammerLoop();

#endif
