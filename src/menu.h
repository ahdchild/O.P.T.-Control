#ifndef MENU_H
#define MENU_H

#include "definitions.h"

//menu ID numbers
    const int MainMenu = -1;
    const byte DistanceMenu = 0;
    const byte SoundMenu = 1;
    const byte WaterMenu = 2;
    const byte LightningMenu = 3;
    const byte IntervalometerMenu = 4;
    const byte SettingsMenu = 5;

// Distance Menu
    const byte DIST_MENU_SIZE = 6;

    // Settings array
    extern int distanceSettings[];

    // index values for settings
    const byte DM_SENSOR = 0;
    const byte DM_DISTANCE = 1;
    const byte DM_WHEN = 2;
    const byte DM_RANGE = 3;
    const byte DM_CONTINUOUS = 4;
    const byte DM_DELAY = 5;

// Sound Menu
    const byte SO_MENU_SIZE = 2;

    // Settings array
    extern int soundSettings[];

    // index values for settings
    const byte SO_THRESHOLD = 0;
    const byte SO_DELAY = 1;

// Water Menu
    const byte WR_MENU_SIZE = 8;

    // Settings array
    extern int waterSettings[];

    // index values for settings
    const byte WR_1_DROP_SIZE = 0;
    const byte WR_1_FLASH_DELAY = 1;
    const byte WR_2_DROP_SIZE = 2;
    const byte WR_2_DROP_DELAY = 3;
    const byte WR_2_FLASH_DELAY = 4;
    const byte WR_3_DROP_SIZE = 5;
    const byte WR_3_DROP_DELAY = 6;
    const byte WR_3_FLASH_DELAY = 7;

// Lightning Menu
    const byte LI_MENU_SIZE = 2;

    // Settings array
    extern int lightningSettings[];

    // index values for settings
    const byte LI_CHANGE_AMT = 0;
    const byte LI_HOLD_SHUTTER = 1;

// Intervalometer Menu
    const byte INT_MENU_SIZE = 8;

    // Settings array
    extern int IntervalometerSettings[];

    // index values for settings
    const byte INT_AUTO = 0;
    const byte INT_TOTAL_SHOTS = 1;
    const byte INT_TOTAL_TIME = 2;
    const byte INT_SHOOT_EVERY = 3;
    const byte INT_INITIAL_DELAY = 4;
    const byte INT_HOLD_SHUTTER = 5;
    const byte INT_TOTAL_TIME_UNITS = 6; // not a seperately visible setting. Toggle with measure
    const byte INT_SHOOT_EVERY_UNITS = 7; // not a seperately visible setting. Toggle with measure

// Settings Menu
    const byte SET_MENU_SIZE = 2;

    // Settings array
    extern int SettingSettings[];
    
    // index values for settings
    const byte SET_TRIGGER = 0;
    const byte SET_LIGHT = 1;

// Lookup charts
    const byte ONOFF_CHART = 1;
    const byte TRIGGER_CHART = 2;
    const byte RANGE_CHART = 3;
    const byte SENSOR_CHART = 4;
    const byte INT_PRIORITY_CHART = 5;
    const byte TIME_UNIT_CHART = 6;
    
 
    // values for which distance sensor is used
    const int USE_LASER = 0;
    const int USE_ULTRASONIC = 1;

    // values for distance trigger condition
    const int GREATER_THAN = 0;
    const int LESS_THAN = 1;
    const int AROUND = 2;
    const int ON_CHANGE = 3;

    // values for intervalometer lock mode
    const int SHOT_COUNT = 0;
    const int TOTAL_TIME = 1;
    const int SHOOT_EVERY = 2;

    // values for time units
    const int SECONDS = 0;
    const int MINUTES = 1;
    const int HOURS = 2;
    const int DAYS = 3;

    // values for trigger chart
    const int FIRE_FLASH = 0;
    const int FIRE_SHUTTER = 1;
    const int LOCKUP = 2;

    // values for on off chart

    const int OFF = 0;
    const int ON = 1;
    const int AUTO = 2;

//menu navigation & display variables
    const byte lastMenu = SettingsMenu; // used to determine when we're at the end of the list
    const byte itemsPerPage = 3;
    extern int currentMenu;
    extern byte currentItem;
    extern byte currentPage; // first menuitem on the screen

    
    //the current menu item being worked with
    extern String currentName;
    extern String currentValue;
    extern int currentMin;
    extern int currentMax;
    extern bool screenReprintNeeded;

    //extern int currentItemMax;
    //extern int currentitemMin;
    //extern byte currentMenuSize;

    const int BUTTON_HOLD_1 = 1600; // hold button to change by larger amoiunt
    const int CHANGE_AMT_1 = 10;
    const int BUTTON_HOLD_2 = 5000; // hold longer for even larger amount
    const int CHANGE_AMT_2 = 70;
    const int CHANGE_RATE = 400; // how often to jump when holding the button

//Functions
    void defaultSettings();

    String menuName(int whichItem); // Retuirn text name of a menu
    void menuItem(int whichItem, int action=0);
    void changeCurrentMenu(int whichMenu);
    String decodeItemValue(int value, byte lookupChart);
    void automateIntervalometer();

    void PrintScreen(); // clear the screen. Print each line starting from currentPage
    String PrintMenuLine(byte screenLine); // Print a particular menu line to lcd. Use case switch to pull from a const String array then print the relevant variable.

    void buttonMenuDown();
    void buttonMenuUp();
    void buttonItemLeft(bool holdDown = false);
    void buttonItemRight(bool holdDown = false);
    void buttonCancel();
    void buttonMeasure();
 
    int limitValue(int value); // confine a vaule to currentMax and currentMin
    String simplifySeconds(int seconds);

#endif