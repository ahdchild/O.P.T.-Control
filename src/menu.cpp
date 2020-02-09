#include "menu.h"

int currentMenu = MainMenu;
byte currentItem = 0;
byte currentPage = 0;
bool screenReprintNeeded = true;
byte cursorPosition = 0;

String currentName;
String currentValue;
int currentMin;
int currentMax;
byte menuLength = 6;

// Distance settings array
int distanceSettings[] = {
    USE_LASER, // default sensor
    100, // default distance
    AROUND, // default trigger condition
    10 // default range
};

String menuName() {
    String theName;

    switch (currentMenu)
    {
    case MainMenu:
        theName = "Main Menu";
        break;
    case DistanceMenu:
        theName = "Distance Trig";
        break;
    case SoundMenu:
        theName = "Sound Trig";
        break;
    case WaterMenu:
        theName = "Waterdrops";
        break;
    case LightningMenu:
        theName = "Lightning Trig";
        break;
    case IntervalometerMenu:
        theName = "Intervalometer";
        break;
    case SettingsMenu:
        theName = "Settings";
        break;
    default:
        theName = "Error";
        break;
    }
    return theName;
}

// clear the screen. Print each line starting from currentPage
void PrintScreen() {
    // do nothing if no reprint is needed
    if (!screenReprintNeeded) return;
    
    screenReprintNeeded = false;

    byte lineLength; 
    byte i;
    String lineToPrint;

    lcd.home();

    // print top line of menu
    lineToPrint = "** " + menuName();
    lineToPrint = lineToPrint + " ";
    lineLength = lcd.print(shortenString(lineToPrint));
    // fill the rest of the line with space
    for (i=lineLength; i < 20; i++) lcd.print("*");

    // print the next 3 lines
    for (int j=1; j<4; j++){
        lcd.setCursor(0,j);
        lineToPrint = PrintMenuLine(j-1);
        lineLength = lcd.print(shortenString(lineToPrint));
        // fill the rest of the line with space
        for (i=lineLength; i < 20; i++) lcd.print(" ");
    }

    lcd.setCursor(cursorPosition, (currentItem-currentPage) + 1);
    lcd.blink();
    
}

 // Print a particular menu line to lcd. Use case switch to pull from a const String array then print the relevant variable.
String PrintMenuLine(byte screenLine) {
    String theLine;
    int whichItem = currentPage + screenLine;
    
    menuItem(whichItem);
    if (whichItem == currentItem) cursorPosition = currentName.length() - 1;
    if (cursorPosition > 19) cursorPosition = 19;
    theLine = currentName + currentValue;

    return theLine;
}


void menuItem(int whichItem, int action) {
    //to do - make sure whichItem is not outside the menu count
    
    //Main Menu
    if (currentMenu == MainMenu) {
        currentValue="";
        currentMin = 0;
        currentMax = 0;
        if (action != 0) changeCurrentMenu(whichItem);
        switch (whichItem)
        {
        case DistanceMenu:
            currentName = "Distance Trigger -> ";
            break;
        case SoundMenu:
            currentName = "Sound Trigger -> ";
            break;
        case WaterMenu:
            currentName = "Waterdrop -> ";
            break;
        case LightningMenu:
            currentName = "Lightning -> ";
            break;
        case IntervalometerMenu:
            currentName = "Intervalometer -> ";
            break;
        case SettingsMenu:
            currentName = "Settings -> ";
            break;                            
        default:
            currentName = "Item Error";
            currentValue = "333";
            break;
        }
    }

    //Distance Menu
    if (currentMenu == DistanceMenu) {
        switch (whichItem)
        {
        case 0:
            currentName = "Sensor: ";
            currentMin = 0;
            currentMax = 1;
            currentValue = decodeItemValue(distanceSettings[DM_SENSOR], SENSOR_CHART);
            break;
        case 1:
            currentName = "Distance: ";
            currentMin = 1;
            currentMax = 400;
            currentValue = distanceSettings[DM_DISTANCE];
            break;
        case 2:
            currentName = "When: ";
            currentMin = 0;
            currentMax = 3;
            currentValue = decodeItemValue(distanceSettings[DM_WHEN], RANGE_CHART);
            break;
        case 3:
            currentName = "Range for =: ";
            currentMin = 1;
            currentMax = 100;
            currentValue = distanceSettings[DM_RANGE];
            break;                            
        default:
            currentName = "Item Error";
            currentValue = "123";
            break;
        }
    }   
}

String decodeItemValue(int value, byte lookupChart) {

    String *conversionChart;
    String theText;
    
    switch (lookupChart) {
        case ONOFF_CHART:
            conversionChart = new String[3]{"off", "on", "auto"};
            break;
        case TRIGGER_CHART:
            conversionChart = new String[3]{"Flash", "Shutter", "Lockup"};
            break;
        case RANGE_CHART:
            conversionChart = new String[3]{"Dist > x", "Dist < x", "Dist = x"};
            break;
        case SENSOR_CHART:
            conversionChart = new String[2] {"Laser", "Sound"};
            break;
        case INT_PRIORITY_CHART:
            conversionChart = new String[3]{"Shots", "Duration", "Every"};
            break;
        
        default:
            break;
    }
    theText = conversionChart[value];
    delete[] conversionChart;
    return theText;
}

void buttonMenuDown() {
    if (mode == STANDBY_MODE) {
        if (currentItem > 0) {
            currentItem--;
            if (currentItem < currentPage) currentPage--; 
            screenReprintNeeded = true;
        }
    }
}

void buttonMenuUp() {
    if (mode == STANDBY_MODE) {
        if (currentItem < menuLength - 1) {
            currentItem++;
            if (currentItem - currentPage >= itemsPerPage) currentPage++; 
            screenReprintNeeded = true;   
        }
    }
}

void changeCurrentMenu(int whichMenu) {
    int menuSizes[] = {
        6, // MainMenu 
        4, // DistanceMenu 
        1, // SoundMenu 
        1, // WaterMenu 
        1, // LightningMenu 
        1, // IntervalometerMenu 
        1 // SettingsMenu 
    };
    if (whichMenu >= MainMenu && whichMenu <= lastMenu) {
        if (whichMenu == MainMenu && currentMenu > MainMenu){
            currentItem = currentMenu;
            currentPage = currentMenu;
            if (lastMenu-itemsPerPage < currentPage) currentPage = (lastMenu-itemsPerPage)+1;
        } else {
            currentItem = 0; 
            currentPage = 0;
        }
        currentMenu = whichMenu;
        menuLength = menuSizes[whichMenu+1]; // Add 1 since main menu is -1

        screenReprintNeeded = true;
    }
}

void buttonItemLeft() {
    if (mode == STANDBY_MODE) {
        menuItem(currentItem, -1);
    }
}

void buttonItemRight() {
    if (mode == STANDBY_MODE) {
        menuItem(currentItem, 1);
    }
}

void buttonCancel() {
    if (mode == STANDBY_MODE && currentMenu != MainMenu) {
        changeCurrentMenu(MainMenu);
    }
}
/*

void PrintMenu(byte whichItem, bool isActive); // Print a particular menu line to lcd. Use case switch to pull from a const String array then print the relevant variable.
void SetValue(int value); //Set value for a current menu item first check the value against currentItemMax and currentitemMin, then use switch case to set whichever global variable
void DoAction(); //Do action related to current menu item.
void SetMenu(byte whichMenu); //change the current menu. Check against totalMenus. Update currentMenuSize
void SetItem(byte whichItem); // change the currentItem. Update currentItemMax and currentitemMin



*/