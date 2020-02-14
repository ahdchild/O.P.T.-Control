#include "menu.h"

// Track current meenu and menu state
int currentMenu = MainMenu;
byte currentItem = 0;
byte currentPage = 0;
byte cursorPosition = 0;
byte menuLength = lastMenu + 1; // how many items on current menu. initialize with main menu

// Should screen be reprinted
bool screenReprintNeeded = true;

// Track values for currentl item
String currentName;
String currentValue;
int currentMin;
int currentMax;

// Track how long buttons were held to change values
double buttonHoldStart;
double buttonHoldLastAction;

// Settings array for each menu
int distanceSettings[DIST_MENU_SIZE];
int soundSettings[SO_MENU_SIZE];
int waterSettings[WR_MENU_SIZE];
int lightningSettings[LI_MENU_SIZE];
int IntervalometerSettings[INT_MENU_SIZE];
int SettingSettings[SET_MENU_SIZE];


// FUNCTIONS
void defaultSettings() {
    distanceSettings[DM_SENSOR] = USE_LASER;
    distanceSettings[DM_DISTANCE] = 100;
    distanceSettings[DM_WHEN] = AROUND;
    distanceSettings[DM_RANGE] = 10;
    distanceSettings[DM_CONTINUOUS] = ON;
    distanceSettings[DM_DELAY] = 0;


    soundSettings[SO_THRESHOLD] = 140;
    soundSettings[SO_DELAY] = 0;

    waterSettings[WR_1_DROP_SIZE] = 10;
    waterSettings[WR_1_FLASH_DELAY] = 10;
    waterSettings[WR_2_DROP_DELAY] = 0;
    waterSettings[WR_2_DROP_SIZE] = 0;
    waterSettings[WR_2_FLASH_DELAY] = 0;
    waterSettings[WR_3_DROP_DELAY] = 0;
    waterSettings[WR_3_DROP_SIZE] = 0;
    waterSettings[WR_3_FLASH_DELAY] = 0;

    lightningSettings[LI_CHANGE_AMT] = 10;
    lightningSettings[LI_HOLD_SHUTTER] = 0;

    IntervalometerSettings[INT_AUTO] = SHOT_COUNT;
    IntervalometerSettings[INT_TOTAL_SHOTS] = 12;
    IntervalometerSettings[INT_TOTAL_TIME] = 120;
    IntervalometerSettings[INT_SHOOT_EVERY] = 10;
    IntervalometerSettings[INT_INITIAL_DELAY] = 0;
    IntervalometerSettings[INT_HOLD_SHUTTER] = 0;
    IntervalometerSettings[INT_TOTAL_TIME_UNITS] = SECONDS;
    IntervalometerSettings[INT_SHOOT_EVERY_UNITS] = SECONDS;
    
    SettingSettings[SET_TRIGGER] = FIRE_FLASH;
    SettingSettings[SET_LIGHT] = AUTO;
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
    menuSizes[1+WaterMenu] = WR_MENU_SIZE; 
    menuSizes[1+LightningMenu] = LI_MENU_SIZE; 
    menuSizes[1+IntervalometerMenu] = INT_MENU_SIZE; 
    menuSizes[1+SettingsMenu] = SET_MENU_SIZE; 
    
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
        return;
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
            currentValue = String(distanceSettings[whichItem]) + "cm";
            if (distanceSettings[DM_WHEN] == ON_CHANGE) currentValue = "Off";
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
            if (distanceSettings[DM_WHEN] < AROUND) currentValue = "Off";
            break;
        case DM_CONTINUOUS:
            currentName = "Continuous: ";
            currentMin = 0;
            currentMax = 1;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = decodeItemValue(distanceSettings[whichItem], ONOFF_CHART);
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
        return;
    }

    //Sound Menu
    if (currentMenu == SoundMenu) {
        switch (whichItem)
        {
        case SO_THRESHOLD:
            currentName = "Threshold: ";
            currentMin = 0;
            currentMax = 255;
            if (action != 0) soundSettings[whichItem] = limitValue(soundSettings[whichItem] + action);
            currentValue = soundSettings[whichItem];
            break;
        case SO_DELAY:
            currentName = "Delay: ";
            currentMin = 0;
            currentMax = 1000;
            if (action != 0) soundSettings[whichItem] = limitValue(soundSettings[whichItem] + action);
            currentValue = soundSettings[whichItem];
            break;
        default:
            currentName = "";
            currentValue = "";
        }
        return;
    }

    // Water Menu
    if (currentMenu == WaterMenu) {
        switch (whichItem)
        {
        case WR_1_DROP_SIZE:
            currentName = "Drop 1 Size: ";
            currentMin = 1;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            currentValue = String(waterSettings[whichItem]) + "ms";
            break;
        case WR_1_FLASH_DELAY:
            currentName = "Flash Delay 1: ";
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            currentValue = waterSettings[whichItem];
            break;
        case WR_2_DROP_SIZE:
            currentName = "Drop 2 Size: ";
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            currentValue = waterSettings[whichItem];
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
                currentValue = "Off";
            }
            break;
        case WR_2_DROP_DELAY:
            currentName = "Drop 2 Delay: ";
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        case WR_2_FLASH_DELAY:
            currentName = "Flash Delay 2: ";
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        case WR_3_DROP_SIZE:
            currentName = "Drop 3 Size: ";
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            if (waterSettings[WR_3_DROP_SIZE] == 0) {
                for (int i = WR_3_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
                currentValue = "Off";
            }
            break;
        case WR_3_DROP_DELAY:
            currentName = "Drop 3 Delay: ";
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_3_DROP_SIZE] == 0) {
                for (int i = WR_3_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        case WR_3_FLASH_DELAY:
            currentName = "Flash Delay 3: ";
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_3_DROP_SIZE] == 0) {
                for (int i = WR_3_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        default:
            currentName = "";
            currentValue = "";
        }
        return;
    }
    
    //Lightning Menu
    if (currentMenu == LightningMenu) {
        switch (whichItem)
        {
        case LI_CHANGE_AMT:
            currentName = "Light Change: ";
            currentMin = 1;
            currentMax = 100;
            if (action != 0) lightningSettings[whichItem] = limitValue(lightningSettings[whichItem] + action);
            currentValue = lightningSettings[whichItem];
            break;
        case LI_HOLD_SHUTTER:
            currentName = "Hold Shutter: ";
            currentMin = 0;
            currentMax = 60;
            if (action != 0) lightningSettings[whichItem] = limitValue(lightningSettings[whichItem] + action);
            currentValue = lightningSettings[whichItem];
            if (currentValue == "0") currentValue = "Off";
            break;
        default:
            currentName = "";
            currentValue = "";
        }
        return;
    }

    //Intervalometer Menu
    if (currentMenu == IntervalometerMenu) {
        switch (whichItem)
        {
        case INT_AUTO:
            currentName = "Automate: ";
            currentMin = 0;
            currentMax = 2;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            currentValue = decodeItemValue(IntervalometerSettings[whichItem], INT_PRIORITY_CHART);
            break;
        case INT_TOTAL_SHOTS:
            currentName = "Total Shots: ";
            currentMin = 0;
            currentMax = 10000;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            automateIntervalometer();
            currentValue = IntervalometerSettings[whichItem];
            break;
        case INT_TOTAL_TIME:
            currentName = "Total Time: ";
            currentMin = 0;
            currentMax = 10000;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            automateIntervalometer();
            currentValue = IntervalometerSettings[whichItem];
            break;
        case INT_SHOOT_EVERY:
            currentName = "Shoot Every: ";
            currentMin = 0;
            currentMax = 3600;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            automateIntervalometer();
            currentValue = IntervalometerSettings[whichItem];
            break;
        case INT_INITIAL_DELAY:
            currentName = "Initial Delay: ";
            currentMin = 0;
            currentMax = 3600;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            currentValue = IntervalometerSettings[whichItem];
            if (currentValue == "0") currentValue = "Off";
            break;
        case INT_HOLD_SHUTTER:
            currentName = "Hold Shutter: ";
            currentMin = 0;
            currentMax = 60;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            currentValue = IntervalometerSettings[whichItem];
            if (currentValue == "0") currentValue = "Off";
            break;
        default:
            currentName = "";
            currentValue = "";
        }
        return;
    }
    
    //Settings Menu
    if (currentMenu == SettingsMenu) {
        switch (whichItem)
        {
        case SET_TRIGGER:
            currentName = "Control: ";
            currentMin = 0;
            currentMax = 1;
            if (action != 0) SettingSettings[whichItem] = limitValue(SettingSettings[whichItem] + action);
            currentValue = decodeItemValue(SettingSettings[whichItem], TRIGGER_CHART); 
            break;
        case SET_LIGHT:
            currentName = "Light: ";
            currentMin = 0;
            currentMax = 2;
            if (action != 0) SettingSettings[whichItem] = limitValue(SettingSettings[whichItem] + action);
            currentValue = decodeItemValue(SettingSettings[whichItem], ONOFF_CHART);
            if (SettingSettings[whichItem] != OFF) lightsOn();
            else lightsOff();
            break;
        default:
            currentName = "";
            currentValue = "";
        }
        return;
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
            conversionChart = new String[3]{"Shot Count", "Total Time", "Time Btwn"};
            break;
        case TIME_UNIT_CHART:
            conversionChart = new String[4]{" secs", " mins", " hrs", " days"};
            break;
        
        
        default:
            break;
    }
    theText = conversionChart[value];
    delete[] conversionChart;
    return theText;
}

void automateIntervalometer() {
    if (IntervalometerSettings[INT_AUTO] == SHOT_COUNT)
        IntervalometerSettings[INT_TOTAL_SHOTS] = IntervalometerSettings[INT_TOTAL_TIME] / IntervalometerSettings[INT_SHOOT_EVERY];
    else if (IntervalometerSettings[INT_AUTO] == TOTAL_TIME)
        IntervalometerSettings[INT_TOTAL_TIME] = IntervalometerSettings[INT_TOTAL_SHOTS] * IntervalometerSettings[INT_SHOOT_EVERY];
    else if (IntervalometerSettings[INT_AUTO] == SHOOT_EVERY)
        IntervalometerSettings[INT_SHOOT_EVERY] = IntervalometerSettings[INT_TOTAL_TIME] / IntervalometerSettings[INT_TOTAL_SHOTS];
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
}

void buttonCancel() {
    if (mode == STANDBY_MODE && currentMenu != MainMenu) {
        changeCurrentMenu(MainMenu);
    } else if (mode != STANDBY_MODE) {
        mode = CANCELLING_MODE;
    }
}

void buttonMeasure() {
    if (mode != STANDBY_MODE) return;

    if (currentMenu == DistanceMenu && currentItem == DM_DISTANCE) {
        distanceSettings[DM_DISTANCE] = getDistance();
        screenReprintNeeded = true;
    }

    if (currentMenu == SoundMenu && currentItem == SO_THRESHOLD) {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Shhh! I'm listening!");
        lcd.setCursor(0,2);
        for (int i = 0; i < 20; i++) {
            lcd.print(".");
            delay(50);
        }
        soundSettings[SO_THRESHOLD] = analogRead(SOUND_SENSOR);
        screenReprintNeeded = true;
    }

    if (currentMenu == WaterMenu) {
        if (currentItem == WR_1_DROP_SIZE || currentItem == WR_2_DROP_SIZE || currentItem == WR_3_DROP_SIZE) {
            createWaterdrop(waterSettings[currentItem]);
        }
    }
}

int limitValue(int value) {
    if (value < currentMin) value = currentMin;
    if (value > currentMax) value = currentMax;

    return value;
}

String simplifySeconds(int seconds) {
    String theText = ""; 
    if (seconds < 60) theText =  seconds + " secs";
    else if (seconds < 3600) theText = seconds / 60 + " mins";
    else  theText = seconds / 3600 + " hrs";

    return theText;
}