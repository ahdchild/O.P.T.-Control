#include <Arduino.h>
#include "ahd_menu.h"

const int SCREEN_SIZE = 2; //how many rows the screen has

String onOff[] = {"off", "on", "auto"};

menuitem testMenuItems[] = {
    menuitem("first"), 
    menuitem("second"),
    menuitem("third", onOff, 2),
    menuitem("third", onOff, 3)};

menu mainMenu("Main", testMenuItems, 4);