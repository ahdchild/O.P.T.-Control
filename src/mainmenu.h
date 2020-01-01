#include <Arduino.h>
#include "ahd_menu.h"

//how many rows the screen has
const int SCREEN_SIZE = 2;

//Give names and unique IDs to actions
#define GET_DISTANCE 100
#define GET_SOUND_LEVEL 101
#define CHECK_RAM 102
#define TRIGGER_BY_DISTANCE 103
#define TRIGGER_BY_SOUND 104
#define TRIGGER_BY_WATERDROP 105
#define TRIGGER_BY_LIGHTING 106
#define RUN_INTEVALOMETER 107
#define RUN_TIMER 108

// Text options for menu items
String onOffChoices[] = {"off", "on", "auto"};
String triggerChoices[] = {"Flash", "Shutter", "Lockup"};
String rangeChoices[] = {"> x", "< x", "= x"};
String distanceChoices[] = {"Laser", "Sound"};
String intPriorityChoices[] = {"Shots", "Duration", "Every"};

//Main menu should always be index 0
const byte MainMenuIndex = 0;
const byte DistanceMenuIndex = 1;
const byte SoundMenuIndex = 2;
const byte WaterMenuIndex = 3;
const byte LightningMenuIndex = 4;
const byte IntervalometerMenuIndex = 5;
const byte SettingsMenuIndex = 6;

//Distance Trigger menu
menuitem DistanceMenuItems[] = {
    menuitem("Use", distanceChoices, 2),
    menuitem("Dist"), 
    menuitem("When", rangeChoices, 3),
    menuitem("+/-"),
    menuitem(GET_DISTANCE, "Measure", true),
    menuitem(0, "Main Menu")
};
const byte DistanceMenuSize = sizeof(DistanceMenuItems)/sizeof(menuitem);

//Sound Trigger menu 
menuitem SoundMenuItems[] = {
    menuitem("Thrshld"),
    menuitem(GET_SOUND_LEVEL, "Get Thrshld", true),
    menuitem(MainMenuIndex, "Main Menu")
};
const byte SoundMenuSize = sizeof(SoundMenuItems)/sizeof(menuitem);

//Waterdrop menu
menuitem WaterMenuItems[] = {
    menuitem("Open Time"),
    menuitem("Drop 2"),
    menuitem("Drop 3"),
    menuitem("Trigger"),
    menuitem(MainMenuIndex, "Main Menu")
};
const byte WaterMenuSize = sizeof(WaterMenuItems)/sizeof(menuitem);

//Lightning menu
menuitem LightningMenuItems[] = {
    menuitem("Max Time"),
    menuitem(MainMenuIndex, "Main Menu")
};
const byte LightningMenuSize = sizeof(LightningMenuItems)/sizeof(menuitem);

//Intervalometer menu
menuitem IntervalometerMenuItems[] = {
    menuitem("Delay"),
    menuitem("Lock", intPriorityChoices, 3),
    menuitem("Shots"),
    menuitem("Duration"),
    menuitem("Every"),
    menuitem(MainMenuIndex, "Main Menu")
};
const byte IntervalometerMenuSize = sizeof(IntervalometerMenuItems)/sizeof(menuitem);
//Settings menu
menuitem SettingsMenuItems[] = {
    menuitem("Light", onOffChoices, 3),
    menuitem("Trigger", triggerChoices, 3),
    menuitem(0, "Main Menu")
};
const byte SettingsMenuSize = sizeof(SettingsMenuItems)/sizeof(menuitem);

//Main menu
menuitem MainMenuItems[] = {
    menuitem(DistanceMenuIndex, "Dist Trig"),
    menuitem(SoundMenuIndex, "Sound Trig"),
    menuitem(WaterMenuIndex, "Waterdrop"),
    menuitem(LightningMenuIndex, "Lightning"),
    menuitem(IntervalometerMenuSize, "Intervalometer"),
    menuitem(SettingsMenuIndex, "Settings")    
};
const byte MainMenuSize = sizeof(MainMenuItems)/sizeof(menuitem);


//Menu Array used to navigate between main menu and submenus. Items should be ordered based on the MenuIndex constants assigned above
menu menuArray[] = {
    menu("Main Menu", MainMenuItems, MainMenuSize, MainMenuIndex, SCREEN_SIZE),
    menu("Dist Trig", DistanceMenuItems, DistanceMenuSize, DistanceMenuIndex, SCREEN_SIZE),
    menu("Sound Trig", SoundMenuItems, SoundMenuSize, SoundMenuIndex, SCREEN_SIZE),
    menu("Waterdrop", WaterMenuItems, WaterMenuSize, WaterMenuIndex, SCREEN_SIZE),
    menu("Lightning", LightningMenuItems, LightningMenuSize, LightningMenuIndex, SCREEN_SIZE),
    menu("Intervalometer", IntervalometerMenuItems, IntervalometerMenuSize, IntervalometerMenuIndex, SCREEN_SIZE),
    menu("Settings", SettingsMenuItems, SettingsMenuSize, SettingsMenuIndex, SCREEN_SIZE),
};

//Pointer used to access the currently active menu. Change the current menu with currentMenu = &menuArray[currentMenu->currentMenu];
menu *currentMenu = &menuArray[0];