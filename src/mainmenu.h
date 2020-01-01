#include <Arduino.h>
#include "ahd_menu.h"

const int SCREEN_SIZE = 2; //how many rows the screen has

String onOffChoices[] = {"off", "on", "auto"};
String triggerChoices[] = {"Flash", "Shutter", "Lockup"};
String rangeChoices[] = {"> x", "< x", "= x"};
String distanceChoices[] = {"Laser", "Sound"};

const byte MainMenuIndex = 0;

//Distance Trigger menu
const byte DistanceMenuIndex = 1;
menuitem DistanceMenuItems[] = {
    menuitem("Use", distanceChoices, 2),
    menuitem("Dist"), 
    menuitem("When", rangeChoices, 3),
    menuitem("+/-"),
    menuitem("Measure"),
    menuitem(0, "Main Menu")
};
const byte DistanceMenuSize = sizeof(DistanceMenuItems)/sizeof(menuitem);

//Sound Trigger menu
const byte SoundMenuIndex = 2;
menuitem SoundMenuItems[] = {
    menuitem("Thrshld"),
    menuitem("Get Thrshld"),
    menuitem(MainMenuIndex, "Main Menu")
};
const byte SoundMenuSize = sizeof(SoundMenuItems)/sizeof(menuitem);

//Main menu
menuitem MainMenuItems[] = {
    menuitem(DistanceMenuIndex, "Dist Trig"),
    menuitem(SoundMenuIndex, "Sound Trig")
};
const byte MainMenuSize = sizeof(MainMenuItems)/sizeof(menuitem);


//Menu Array
menu menuArray[] = {
    menu("Main Menu", MainMenuItems, MainMenuSize, MainMenuIndex, SCREEN_SIZE),
    menu("Dist Trig", DistanceMenuItems, DistanceMenuSize, DistanceMenuIndex, SCREEN_SIZE),
    menu("Sound Trig", SoundMenuItems, SoundMenuSize, SoundMenuIndex, SCREEN_SIZE)
};

menu *currentMenu = &menuArray[0]; // change the current menu with currentMenu = &menuArray[currentMenu->currentMenu];