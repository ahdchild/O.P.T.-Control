#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Libraries
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <pcf8574_esp.h>
#include <SimpleDHT.h>
#include <VL53L1X.h>

// Input Pins
const int FLASH_DETECT = 0;
const int CAMERA_DETECT = 11;
const int SOUND_SENSOR = A0;
const int LIGHT_SENSOR = A1;
const int US_ECHO = 8;
const int TEMPERATURE = 1;
const int LASER_INTERRUPT = 2;
const int BUTTON_INTERRUPT = 3;

// Output Pins
const int FLASH_TRIGGER = 7;
const int CAMERA_SHUTTER = 12;
const int CAMERA_FOCUS = 13;
const int US_TRIGGER = 9;
const int WATERDROP = 10;
const int ROOM_LIGHT = 6;

// Constants
const int flashCooldown = 800;
const byte USDistanceAVG = 3;
const byte TempRetries = 10;
const unsigned int TempUpdateFreq = 60000;

// Components
extern LiquidCrystal_I2C lcd;
extern PCF857x buttonController;
extern VL53L1X laserDistance;
// extern SimpleDHT11 tempSensor;
// extern byte temperature;
// extern byte humidity;

// Flags
extern volatile bool PCFInterruptFlag;

// i2c Addresses
/*
    0x20    i2C expander
    0x27    LCD Display
    0x29
*/


// Functions
void testSetup(); //so test code doesn't clutter main code
void testLoop(); //so test code doesn't clutter main code
void initializeStuff(); //initialize pins, modules, etc
void PCFInterrupt(); //Handle button interrupts 
void flash();
void shutterOpen(bool hold=false);
void shutterClose();
void getTemp();
double getUSDistance(bool accurate = true);
double getUSDistanceAVG(bool accurate = true);
String padInt(int number, int spaces);

#endif