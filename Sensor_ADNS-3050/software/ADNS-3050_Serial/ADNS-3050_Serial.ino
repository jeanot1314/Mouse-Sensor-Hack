/*
 * This code read an ADNS-3050 sensor data using a Teensy 3.2 board
 * 
 * It is inspired from Tom101222 code available here https://github.com/Tom101222/Adns-3050-Optical-Sensor/tree/1b433ad21caafd9b150a414805f1c707d291e043 
 * 
 * The function "getImage" had been added to read the 361 pixels of the sensor and send it through Serial
 * Use the Qt GUI on the repo to draw the pixels
 * 
 * See the header ADNS3050.h for the wiring between the sensor and the Teensy board
 * 
 */


#include "ADNS3050.h"
#include <SPI.h>

int cpt;

void setup(){
  startup();
  cpt = 0;
  //SPI.begin();
  //SPI.beginTransaction(SPISettings(36000000, MSBFIRST, SPI_MODE0));
}

void loop() {
//getXY();//Get X and Y accerlation and print
getImage();
//getImageSerial();
delay(10);
}
