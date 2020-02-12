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
    const byte DIST_MENU_SIZE = 5;

    // Settings array
    extern int distanceSettings[];

    // index values for settings
    const byte DM_SENSOR = 0;
    const byte DM_DISTANCE = 1;
    const byte DM_WHEN = 2;
    const byte DM_RANGE = 3;
    const byte DM_DELAY = 4;

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


// Lookup charts
    const byte ONOFF_CHART = 1;
    const byte TRIGGER_CHART = 2;
    const byte RANGE_CHART = 3;
    const byte SENSOR_CHART = 4;
    const byte INT_PRIORITY_CHART = 5;
 
    // values for which distance sensor is used
    const byte USE_LASER = 0;
    const byte USE_ULTRASONIC = 1;

    // values for distance trigger condition
    const byte GREATER_THAN = 0;
    const byte LESS_THAN = 1;
    const byte AROUND = 2;

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

    void PrintScreen(); // clear the screen. Print each line starting from currentPage
    String PrintMenuLine(byte screenLine); // Print a particular menu line to lcd. Use case switch to pull from a const String array then print the relevant variable.

    void buttonMenuDown();
    void buttonMenuUp();
    void buttonItemLeft(bool holdDown = false);
    void buttonItemRight(bool holdDown = false);
    void buttonCancel();
 
    int limitValue(int value); // confine a vaule to currentMax and currentMin

#endif