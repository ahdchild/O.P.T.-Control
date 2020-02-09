#ifndef MENU_H
#define MENU_H

#include "definitions.h"

//menus
    const int MainMenu = -1;
    const byte DistanceMenu = 0;
    const byte SoundMenu = 1;
    const byte WaterMenu = 2;
    const byte LightningMenu = 3;
    const byte IntervalometerMenu = 4;
    const byte SettingsMenu = 5;

//menu navigation & display variables
    const byte lastMenu = SettingsMenu; // used to determine when we're at the end of the list
    const byte itemsPerPage = 3;
    extern int currentMenu;
    extern byte currentItem;
    extern byte currentPage; // first menuitem on the screen

    
    //the menu item being worked with
    extern String currentName;
    extern String currentValue;
    extern int currentMin;
    extern int currentMax;
    

extern bool screenReprintNeeded;

//int currentItemMax;
//int currentitemMin;
//byte currentMenuSize;

// Distance Menu
    // values for which sensor is used
    const byte USE_LASER = 0;
    const byte USE_ULTRASONIC = 1;

    // values for trigger condition
    const byte GREATER_THAN = 0;
    const byte LESS_THAN = 1;
    const byte AROUND = 2;

    // Settings array
    extern int distanceSettings[];

    // index values for settings
    const byte DM_SENSOR = 0;
    const byte DM_DISTANCE = 1;
    const byte DM_WHEN = 2;
    const byte DM_RANGE = 3;


// Lookup charts
    const byte ONOFF_CHART = 1;
    const byte TRIGGER_CHART = 2;
    const byte RANGE_CHART = 3;
    const byte SENSOR_CHART = 4;
    const byte INT_PRIORITY_CHART = 5;

//Functions
    void PrintScreen(); // clear the screen. Print each line starting from currentPage
    String PrintMenuLine(byte screenLine); // Print a particular menu line to lcd. Use case switch to pull from a const String array then print the relevant variable.
    String menuName(int whichItem);

    void buttonMenuDown();
    void buttonMenuUp();
    void buttonItemLeft();
    void buttonItemRight();
    void buttonCancel();

    void menuItem(int whichItem, int action=0);
    String decodeItemValue(int value, byte lookupChart);
    void changeCurrentMenu(int whichMenu);


    void SetMenu(byte whichMenu); //change the current menu. Check against totalMenus. Update currentMenuSize
    void SetItem(byte whichItem); // change the currentItem. Update currentItemMax and currentitemMin
    void SetValue(int value); //Set value for a current menu item first check the value against currentItemMax and currentitemMin, then use switch case to set whichever global variable
    void DoAction(); //Do action related to current menu item.

 

#endif