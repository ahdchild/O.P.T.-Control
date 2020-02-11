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
double buttonHoldStart;
double buttonHoldLastAction;

// Distance settings array
int distanceSettings[DIST_MENU_SIZE];

// Sound settings array
int soundSettings[SO_MENU_SIZE];

void defaultSettings() {
    distanceSettings[DM_SENSOR] = USE_LASER;
    distanceSettings[DM_DISTANCE] = 100;
    distanceSettings[DM_WHEN] = AROUND;
    distanceSettings[DM_RANGE] = 10;
    distanceSettings[DM_DELAY] = 0;

    soundSettings[SO_THRESHOLD] = 140;
    soundSettings[SO_DELAY] = 0;
}

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

void changeCurrentMenu(int whichMenu) {
    int menuSizes[lastMenu+2]; //offset by 2 because 6 menu uitems are numbered -1 through 5

    // offset menu index by 1 to account for main menu at -1
    menuSizes[1+MainMenu] = lastMenu + 1; 
    menuSizes[1+DistanceMenu] = DIST_MENU_SIZE; 
    menuSizes[1+SoundMenu] = SO_MENU_SIZE; 
    menuSizes[1+WaterMenu] = 1; 
    menuSizes[1+LightningMenu] = 1; 
    menuSizes[1+IntervalometerMenu] = 1; 
    menuSizes[1+SettingsMenu] = 1; 
    
    // check to see if whichMenu is in bounds before changing
    if (whichMenu >= MainMenu && whichMenu <= lastMenu) {
        // if changing to the main menu, let current item be the menu we're leaving
        if (whichMenu == MainMenu && currentMenu > MainMenu){
            currentItem = currentMenu;
            currentPage = currentMenu;
            // make sure screen is not scrolled down past the end
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

void menuItem(int whichItem, int action) {
    //to do - make sure whichItem is not outside the menu count
    
    if (action != 0) screenReprintNeeded = true;
    
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
        case DM_SENSOR:
            currentName = "Sensor: ";
            currentMin = 0;
            currentMax = 1;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = decodeItemValue(distanceSettings[whichItem], SENSOR_CHART);
            break;
        case DM_DISTANCE:
            currentName = "Distance: ";
            currentMin = 1;
            currentMax = 400;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = distanceSettings[whichItem];
            break;
        case DM_WHEN:
            currentName = "When: ";
            currentMin = 0;
            currentMax = 3;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = decodeItemValue(distanceSettings[whichItem], RANGE_CHART);
            break;
        case DM_RANGE:
            currentName = "+/- Range: ";
            currentMin = 1;
            currentMax = 100;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = distanceSettings[whichItem];
            break;
        case DM_DELAY:
            currentName = "Delay: ";
            currentMin = 0;
            currentMax = 1000;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = distanceSettings[whichItem];
            break;                                         
        default:
            currentName = "Item Error";
            currentValue = "123";
            break;
        }
    }

    //Sound Menu
    if (currentMenu == SoundMenu) {
        switch (whichItem)
        {
        case 0:
            currentName = "Threshold: ";
            currentMin = 0;
            currentMax = 255;
            currentValue = soundSettings[SO_THRESHOLD];
            break;
        case 1:
            currentName = "Delay: ";
            currentMin = 0;
            currentMax = 1000;
            currentValue = distanceSettings[SO_DELAY];
            break;
        default:
            currentName = "";
            currentValue = "";
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
            conversionChart = new String[4]{"Dist > x", "Dist < x", "Dist = x", "Change"};
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

// Print each line starting from currentPage
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
    int whichItem = currentPage + screenLine; // figure out which menu item is needed
    
    menuItem(whichItem); // update menu variable with values of the correct item
    if (whichItem == currentItem) cursorPosition = currentName.length() - 1;
    if (cursorPosition > 19) cursorPosition = 19;
    theLine = currentName + currentValue;

    return theLine;
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

void buttonItemLeft(bool holdDown) {
    if (mode == STANDBY_MODE) {
        int changeAmt = -1;

        if (holdDown) {
            int timeHeld = millis() - buttonHoldStart;
            int timeSince = millis() - buttonHoldLastAction;

            if (timeHeld > BUTTON_HOLD_2) changeAmt *= CHANGE_AMT_2;
                else if (timeHeld > BUTTON_HOLD_1) changeAmt *= CHANGE_AMT_1;
            
            if (timeSince < CHANGE_RATE) changeAmt = 0;
                else buttonHoldLastAction = millis();
        } else {
            buttonHoldStart = buttonHoldLastAction = millis();
        }
        if (changeAmt != 0) menuItem(currentItem, changeAmt);
    }
}

void buttonItemRight(bool holdDown) {
    if (mode == STANDBY_MODE) {
        int changeAmt = 1;

        if (holdDown) {
            int timeHeld = millis() - buttonHoldStart;
            int timeSince = millis() - buttonHoldLastAction;

            if (timeHeld > BUTTON_HOLD_2) changeAmt *= CHANGE_AMT_2;
                else if (timeHeld > BUTTON_HOLD_1) changeAmt *= CHANGE_AMT_1;
            
            if (timeSince < CHANGE_RATE) changeAmt = 0;
                else buttonHoldLastAction = millis();
        } else {
            buttonHoldStart = millis();
            buttonHoldLastAction = millis();
        }
        if (changeAmt != 0) menuItem(currentItem, changeAmt);
    }

/*     if (mode == STANDBY_MODE) {
        menuItem(currentItem, 1);
    } */
}

void buttonCancel() {
    if (mode == STANDBY_MODE && currentMenu != MainMenu) {
        changeCurrentMenu(MainMenu);
    }
}

int limitValue(int value) {
    if (value < currentMin) value = currentMin;
    if (value > currentMax) value = currentMax;

    return value;
}