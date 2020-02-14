#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Libraries
    #include <Arduino.h>
    #include <LiquidCrystal_I2C.h>
    #include <Wire.h>
    #include <pcf8574_esp.h>
    #include <SimpleDHT.h>
    #include <VL53L1X.h>
    #include "menu.h"
    #include "actions.h"

// Input Pins
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

// Assorted Constants
    const int flashCooldown = 800; // minimum time between firing the flash
    const byte USDistanceAVG = 3;
    const byte TempRetries = 10;
    const unsigned int TempUpdateFreq = 60000;
    const unsigned int RestBetweenShots = 2000;

// Modes - current status of the OPT Control
    extern byte mode;
    const byte STANDBY_MODE = 1;
    const byte CANCELLING_MODE = 2;
    const byte DISTANCE_MODE = 3; // Shooting with the distance trigger
    const byte SOUND_MODE = 4; // Shooting with the sound trigger
    const byte WATER_MODE = 5; // Running a waterdrop shoot
    const byte LIGTNING_MODE = 6; // Shooting lightning shots
    const byte INTERVAL_MODE = 7; // Shooting in intervalometer mode
    

// Components
    extern LiquidCrystal_I2C lcd;
    extern PCF857x buttonController;
    extern VL53L1X laserDistance;
    extern SimpleDHT11 tempSensor;

// Flags
    // extern volatile bool buttonInterruptFlag;
    // extern byte buttonState;

// i2c Addresses
    /*
        0x20    i2C expander
        0x27    LCD Display
        0x29    VL53L1X Laser Distance Sensor
    */

// Buttons
    const byte BT_CANCEL = 0;
    const byte BT_SHOOT = 1;
    const byte BT_MEASURE = 2;
    const byte BT_OK = 3;
    const byte BT_RIGHT = 4;
    const byte BT_LEFT = 5;
    const byte BT_DOWN = 6;
    const byte BT_UP = 7;

// Functions
    void initializeStuff(); //initialize pins, modules, etc
    void PCFInterrupt(); //Handle button interrupts 
    void testSetup(); //so test code doesn't clutter main code
    void testLoop(); //so test code doesn't clutter main code
    void handleButton(); // debounce button presses and trigger events

//Functions - Helpers
    String padInt(int number, int spaces); // take an integer and return it as a string with a set minimum length
    String shortenString(String text, byte length = 20); // Shorten a string

#endif