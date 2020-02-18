#include "menu.h"

// Track current meenu and menu state
int currentMenu = MainMenu;
byte currentItem = 0;
byte currentPage = 0;
byte cursorPosition = 0;
byte menuLength = lastMenu + 1; // how many items on current menu. initialize with main menu
int previousMenu = MainMenu;

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

    IntervalometerSettings[INT_SECS] = 10;
    IntervalometerSettings[INT_MINS] = 0;
    IntervalometerSettings[INT_HRS] = 0;
    IntervalometerSettings[INT_TOTAL_SHOTS] = 2;
    IntervalometerSettings[INT_TOTAL_TIME] = 0;
    IntervalometerSettings[INT_INITIAL_DELAY] = 0;  
    
    SettingSettings[SET_TRIGGER] = FIRE_FLASH;
    SettingSettings[SET_LIGHT] = AUTO;
    SettingSettings[SET_SCREEN] = AUTO;
}

String menuName() {
    String theName;

    switch (currentMenu)
    {
    case MainMenu:
        theName = F("Main Menu");
        break;
    case DistanceMenu:
        theName = F("Distance Trig");
        break;
    case SoundMenu:
        theName = F("Sound Trig");
        break;
    case WaterMenu:
        theName = F("Waterdrops");
        break;
    case LightningMenu:
        theName = F("Lightning Trig");
        break;
    case IntervalometerMenu:
        theName = F("Intervalometer");
        break;
    case SettingsMenu:
        theName = F("Settings");
        break;
    default:
        theName = F("Error");
        break;
    }
    return theName;
}

void changeCurrentMenu(int whichMenu) {
    int menuSizes[lastMenu+2]; //offset by 2 because 6 menu uitems are numbered -1 through 5

    // offset menu index by 1 to account for main menu at -1
    menuSizes[1+MainMenu] = lastMenu + 1; 
    menuSizes[1+DistanceMenu] = DIST_MENU_SIZE + 1; // add extra 1 to show settings menu at the end
    menuSizes[1+SoundMenu] = SO_MENU_SIZE + 1; // add extra 1 to show settings menu at the end
    menuSizes[1+WaterMenu] = WR_MENU_SIZE + 1; // add extra 1 to show settings menu at the end
    menuSizes[1+LightningMenu] = LI_MENU_SIZE + 1; // add extra 1 to show settings menu at the end
    menuSizes[1+IntervalometerMenu] = INT_MENU_SIZE + 1; // add extra 1 to show settings menu at the end
    menuSizes[1+SettingsMenu] = SET_MENU_SIZE; 
    
    // check to see if whichMenu is in bounds before changing
    if (whichMenu >= MainMenu && whichMenu <= lastMenu) {
        // if changing to the main menu, let current item be the menu we're leaving
        if (whichMenu == MainMenu && currentMenu > MainMenu){
            currentItem = currentMenu;
            currentPage = currentMenu;
            // make sure screen is not scrolled down past the end
            if (lastMenu-itemsPerPage < currentPage) currentPage = (lastMenu-itemsPerPage)+1;
        } else if (whichMenu > MainMenu && whichMenu < SettingsMenu && currentMenu != MainMenu) {
            // if returning from settings submenu, set the cursor at the settings menu
            currentItem = menuSizes[whichMenu+1] - 1;
            currentPage = menuSizes[whichMenu+1] - 3;
            if (currentPage < 0) currentPage = 0;
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
            currentName = F("Distance Trigger -> ");
            break;
        case SoundMenu:
            currentName = F("Sound Trigger -> ");
            break;
        case WaterMenu:
            currentName = F("Waterdrop -> ");
            break;
        case LightningMenu:
            currentName = F("Lightning -> ");
            break;
        case IntervalometerMenu:
            currentName = F("Intervalometer -> ");
            break;
        case SettingsMenu:
            currentName = F("Settings -> ");
            break;                            
        default:
            currentName = "";
            currentValue = "";
            break;
        }
        return;
    }

    //Distance Menu
    if (currentMenu == DistanceMenu) {
        switch (whichItem)
        {
        case DM_SENSOR:
            currentName = F("Sensor: ");
            currentMin = 0;
            currentMax = 1;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = decodeItemValue(distanceSettings[whichItem], SENSOR_CHART);
            break;
        case DM_DISTANCE:
            currentName = F("Distance: ");
            currentMin = 1;
            currentMax = 400;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = String(distanceSettings[whichItem]) + "cm";
            if (distanceSettings[DM_WHEN] == ON_CHANGE) currentValue = "Off";
            break;
        case DM_WHEN:
            currentName = F("When: ");
            currentMin = 0;
            currentMax = 3;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = decodeItemValue(distanceSettings[whichItem], RANGE_CHART);
            break;
        case DM_RANGE:
            currentName = F("+/- Range: ");
            currentMin = 1;
            currentMax = 100;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = distanceSettings[whichItem];
            if (distanceSettings[DM_WHEN] < AROUND) currentValue = "Off";
            break;
        case DM_CONTINUOUS:
            currentName = F("Continuous: ");
            currentMin = 0;
            currentMax = 1;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = decodeItemValue(distanceSettings[whichItem], ONOFF_CHART);
            break;                               
        case DM_DELAY:
            currentName = F("Delay: ");
            currentMin = 0;
            currentMax = 1000;
            if (action != 0) distanceSettings[whichItem] = limitValue(distanceSettings[whichItem] + action);
            currentValue = distanceSettings[whichItem];
            break;                                         
        default:
            settingsSubMenu(action);
            break;
        }
        return;
    }

    //Sound Menu
    if (currentMenu == SoundMenu) {
        switch (whichItem)
        {
        case SO_THRESHOLD:
            currentName = F("Threshold: ");
            currentMin = 0;
            currentMax = 255;
            if (action != 0) soundSettings[whichItem] = limitValue(soundSettings[whichItem] + action);
            currentValue = soundSettings[whichItem];
            break;
        case SO_DELAY:
            currentName = F("Delay: ");
            currentMin = 0;
            currentMax = 1000;
            if (action != 0) soundSettings[whichItem] = limitValue(soundSettings[whichItem] + action);
            currentValue = soundSettings[whichItem];
            break;
        default:
            settingsSubMenu(action);
        }
        return;
    }

    // Water Menu
    if (currentMenu == WaterMenu) {
        switch (whichItem)
        {
        case WR_1_DROP_SIZE:
            currentName = F("Drop 1 Size: ");
            currentMin = 1;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            currentValue = String(waterSettings[whichItem]) + "ms";
            break;
        case WR_1_FLASH_DELAY:
            currentName = F("Flash Delay 1: ");
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            currentValue = waterSettings[whichItem];
            break;
        case WR_2_DROP_SIZE:
            currentName = F("Drop 2 Size: ");
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            currentValue = waterSettings[whichItem];
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
                currentValue = F("Off");
            }
            break;
        case WR_2_DROP_DELAY:
            currentName = F("Drop 2 Delay: ");
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        case WR_2_FLASH_DELAY:
            currentName = F("Flash Delay 2: ");
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        case WR_3_DROP_SIZE:
            currentName = F("Drop 3 Size: ");
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_2_DROP_SIZE] == 0) {
                for (int i = WR_2_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            if (waterSettings[WR_3_DROP_SIZE] == 0) {
                for (int i = WR_3_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
                currentValue = F("Off");
            }
            break;
        case WR_3_DROP_DELAY:
            currentName = F("Drop 3 Delay: ");
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_3_DROP_SIZE] == 0) {
                for (int i = WR_3_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        case WR_3_FLASH_DELAY:
            currentName = F("Flash Delay 3: ");
            currentMin = 0;
            currentMax = 500;
            if (action != 0) waterSettings[whichItem] = limitValue(waterSettings[whichItem] + action);
            if (waterSettings[WR_3_DROP_SIZE] == 0) {
                for (int i = WR_3_DROP_SIZE; i < WR_MENU_SIZE; i++) waterSettings[i] = 0;
            }
            currentValue = waterSettings[whichItem];
            break;
        default:
            settingsSubMenu(action);
        }
        return;
    }
    
    //Lightning Menu
    if (currentMenu == LightningMenu) {
        switch (whichItem)
        {
        case LI_CHANGE_AMT:
            currentName = F("Light Change: ");
            currentMin = 1;
            currentMax = 100;
            if (action != 0) lightningSettings[whichItem] = limitValue(lightningSettings[whichItem] + action);
            currentValue = lightningSettings[whichItem];
            break;
        case LI_HOLD_SHUTTER:
            currentName = F("Hold Shutter: ");
            currentMin = 0;
            currentMax = 60;
            if (action != 0) lightningSettings[whichItem] = limitValue(lightningSettings[whichItem] + action);
            currentValue = lightningSettings[whichItem];
            if (currentValue == "0") currentValue = F("Off");
            break;
        default:
            settingsSubMenu(action);
        }
        return;
    }

    //Intervalometer Menu
    if (currentMenu == IntervalometerMenu) {
        switch (whichItem)
        {
        case INT_SECS:
            currentName = F("Interval Seconds: ");
            currentMin = 0;
            currentMax = 59;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            maxIntervalShots();
            currentValue = IntervalometerSettings[whichItem];
            break;           
        case INT_MINS:
            currentName = F("Interval Minutes: ");
            currentMin = 0;
            currentMax = 59;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            maxIntervalShots();
            currentValue = IntervalometerSettings[whichItem];
            break;           
        case INT_HRS:
            currentName = F("Interval Hours: ");
            currentMin = 0;
            currentMax = 24;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            maxIntervalShots();
            currentValue = IntervalometerSettings[whichItem];
            break;                                    
        case INT_TOTAL_SHOTS:
            currentName = F("Total Shots: ");
            currentMin = 1;
            currentMax = maxIntervalShots();
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            currentValue = IntervalometerSettings[whichItem];
            break;
        case INT_TOTAL_TIME:
            currentName = F(" ");
            currentMin = 0;
            currentMax = 0;
            currentValue = F("Duration: ");
            currentValue += showTotalIntTime();
            break;
        case INT_INITIAL_DELAY:
            currentName = F("Initial Delay: ");
            currentMin = 0;
            currentMax = 3600;
            if (action != 0) IntervalometerSettings[whichItem] = limitValue(IntervalometerSettings[whichItem] + action);
            currentValue = IntervalometerSettings[whichItem];
            if (currentValue == "0") currentValue = F("Off");
            break;
        default:
            settingsSubMenu(action);
        }
        return;
    }
    
    //Settings Menu
    if (currentMenu == SettingsMenu) {
        switch (whichItem)
        {
        case SET_TRIGGER:
            currentName = F("Control: ");
            currentMin = 0;
            currentMax = 1;
            if (action != 0) SettingSettings[whichItem] = limitValue(SettingSettings[whichItem] + action);
            currentValue = decodeItemValue(SettingSettings[whichItem], TRIGGER_CHART); 
            break;
        case SET_LIGHT:
            currentName = F("Light: ");
            currentMin = 0;
            currentMax = 2;
            if (action != 0) SettingSettings[whichItem] = limitValue(SettingSettings[whichItem] + action);
            currentValue = decodeItemValue(SettingSettings[whichItem], ONOFF_CHART);
            if (SettingSettings[whichItem] != OFF) lightsOn();
            else lightsOff();
            break;
        case SET_SCREEN:
            currentName = F("Screen: ");
            currentMin = 1;
            currentMax = 2;
            if (action != 0) SettingSettings[whichItem] = limitValue(SettingSettings[whichItem] + action);
            currentValue = decodeItemValue(SettingSettings[whichItem], ONOFF_CHART);
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
            conversionChart = new String[3]{F("Off"), F("On"), F("Auto")};
            break;
        case TRIGGER_CHART:
            conversionChart = new String[3]{F("Flash"), F("Shutter"), F("Lockup")};
            break;
        case RANGE_CHART:
            conversionChart = new String[4]{F("Dist > x"), F("Dist < x"), F("Dist = x"), F("Change")};
            break;
        case SENSOR_CHART:
            conversionChart = new String[2] {F("Laser"), F("Sound")};
            break;
        case INT_PRIORITY_CHART:
            conversionChart = new String[3]{F("Shot Count"), F("Total Time"), F("Time Btwn")};
            break;
        default:
            break;
    }
    theText = conversionChart[value];
    delete[] conversionChart;
    return theText;
}


long intervalTimeInSecs() {
    long oneShotTime = IntervalometerSettings[INT_SECS];
    oneShotTime += 60 * (long)IntervalometerSettings[INT_MINS];
    oneShotTime += 3600 * (long) IntervalometerSettings[INT_HRS];
    return oneShotTime;
}

int maxIntervalShots() {
    long maxTime = 2592000; // 30 days * 86,400 seconds in a day
    long maxShots = maxTime / intervalTimeInSecs();
    if (maxShots > 10000) maxShots = 10000;
    if (maxShots < 1) maxShots = 1;
    if (IntervalometerSettings[INT_TOTAL_SHOTS] > maxShots) IntervalometerSettings[INT_TOTAL_SHOTS] = maxShots;

    return (int) maxShots;
}

String showTotalIntTime() {
    String result = "";
    
    long seconds = intervalTimeInSecs() * (long)IntervalometerSettings[INT_TOTAL_SHOTS];
    seconds -= intervalTimeInSecs();
    seconds += (long)IntervalometerSettings[INT_INITIAL_DELAY];

    long hours = seconds / 3600;
    seconds -= hours*3600;

    long minutes =  seconds / 60;
    seconds -= minutes * 60;

    if (hours < 10) result+= F("0");
    result += String(hours);
    result += F(":");
    if (minutes < 10) result+= F("0");
    result += String(minutes);
    result += F(":");
    if (seconds < 10) result+= F("0");
    result += String(seconds);

    return result;



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
        for (i=lineLength; i < 20; i++) lcd.print(F(" "));
    }

    lcd.setCursor(cursorPosition, (currentItem-currentPage) + 1);
    lcd.blink();
    
}

void settingsSubMenu(int action) {
    currentName = F("Settings -> ");
    currentValue="";
    currentMin = 0;
    currentMax = 0;
    
    if (action != 0) {
        previousMenu = currentMenu;
        changeCurrentMenu(SettingsMenu);
    }
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
        changeCurrentMenu(previousMenu);
        previousMenu = MainMenu;
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
        int averageSound = 0;
        
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(F("Shhh! I'm listening!"));
        delay(500);
        lcd.setCursor(0,2);
        for (int i = 0; i < 20; i++) {
            averageSound += analogRead(SOUND_SENSOR);
            lcd.print(F("."));
            delay(50);
        }
        averageSound = averageSound / 20;
        soundSettings[SO_THRESHOLD] = averageSound;
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