#include <Arduino.h>
#include "ahd_menu.h"

const int SCREEN_SIZE = 2; //how many rows the screen has

String onOffChoices[] = {"off", "on", "auto"};
String triggerChoices[] = {"Flash", "Shutter", "Lockup"};
String rangeChoices[] = {"> x", "< x", "= x"};
String distanceChoices[] = {"Laser", "Sound"};

const byte MainMenuIndex = 0;

//Distance Trigger menu
const byte DistanceMenuSize = 6;
const byte DistanceMenuIndex = 1;
menuitem DistanceMenuItems[DistanceMenuSize] = {
    menuitem("Use", distanceChoices, 2),
    menuitem("Dist"), 
    menuitem("When", rangeChoices, 3),
    menuitem("+/-"),
    menuitem("Measure"),
    menuitem(0, "Main Menu")
};

//Sound Trigger menu
const byte SoundMenuSize = 3;
const byte SoundMenuIndex = 2;
menuitem SoundMenuItems[SoundMenuSize] = {
    menuitem("Thrshld"),
    menuitem("Get Thrshld"),
    menuitem(MainMenuIndex, "Main Menu")
};

//Main menu
const byte MainMenuSize = 2;
menuitem MainMenuItems[MainMenuSize] = {
    menuitem(DistanceMenuIndex, "Dist Trig"),
    menuitem(SoundMenuIndex, "Sound Trig")
};


//Menu Array
menu menuArray[] = {
    menu("Main Menu", MainMenuItems, MainMenuSize, MainMenuIndex, SCREEN_SIZE),
    menu("Dist Trig", DistanceMenuItems, DistanceMenuSize, DistanceMenuIndex, SCREEN_SIZE),
    menu("Sound Trig", SoundMenuItems, SoundMenuSize, SoundMenuIndex, SCREEN_SIZE)
};

menu *currentMenu = &menuArray[0]; // change the current menu with currentMenu = &menuArray[currentMenu->currentMenu];