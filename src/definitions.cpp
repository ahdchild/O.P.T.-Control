#include "definitions.h"

// Components
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
PCF857x buttonController(0x20, &Wire);
SimpleDHT11 tempSensor(TEMPERATURE);
VL53L1X laserDistance;

// 1 byte represents 8 buttons
byte buttonState = B11111111;
volatile bool buttonInterruptFlag = false;
long lastDebounceTime[] = {0,0,0,0,0,0,0,0};
const long debounceDelay = 10;

// Track current operating mode
byte mode = STANDBY_MODE;


//FUNCTIONS

//initialize pins, modules, etc
void initializeStuff() {
    // Set output pins
    pinMode(FLASH_TRIGGER, OUTPUT);
    pinMode(CAMERA_SHUTTER, OUTPUT);
    pinMode(CAMERA_FOCUS, OUTPUT);
    pinMode(US_TRIGGER, OUTPUT);
    pinMode(WATERDROP, OUTPUT);
    pinMode(ROOM_LIGHT, OUTPUT);

    // Set pull up resistors
    pinMode(BUTTON_INTERRUPT, INPUT_PULLUP);


    // Initialize 20x4 LCD
    lcd.begin(20,4);
    lcd.backlight();
    lcd.clear();

    // Initialize i2c expander
    buttonController.begin();
    buttonController.resetInterruptPin();
    attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT), PCFInterrupt, FALLING);
    // Wire.setClock(100000L);

    //initialize laser distance
    //Wire.setClock(400000);
    laserDistance.setTimeout(500);
    if (!laserDistance.init())
    {
        lcd.print("Failed to detect and initialize sensor!");
        while (1);
    }
    laserDistance.setDistanceMode(VL53L1X::Long);
    laserDistance.setMeasurementTimingBudget(50000);
    laserDistance.startContinuous(50);

    // Initialize variables
    getTemp();
    defaultSettings();

    // Start with the lights on
    lightsOn();
}

// Handle button interrupts
void PCFInterrupt() {
  buttonInterruptFlag = true;
}



String padInt(int number, int spaces) {
    if (spaces > 4) spaces = 4;
    if (spaces < 2) spaces = 2;
    
    String result = String(number);
    
    for (int i = spaces - 1; (10^(i-1)) > number; i--){
        result = " " + result;
    }

    return result;
}

void handleButton() {
    // do nothing if interrupt has not been triggered
    if (!buttonInterruptFlag) {
        if (!bitRead(buttonState, BT_LEFT)) {
            buttonItemLeft(true);
        } else if (!bitRead(buttonState, BT_RIGHT)) {
            buttonItemRight(true);
        }
        return;
    }

    buttonInterruptFlag = false;
    bool timePassed[8];

    byte newButtonState = buttonController.read8();

    // Check each button for debounce delay.
    // timePassed[i] is set true if the debounbce time has passed
    for (int i = 0; i < 8; i++) {
        timePassed[i] = ((millis() - lastDebounceTime[i]) > debounceDelay);
        if (timePassed[i]) lastDebounceTime[i] = millis();
    }

    // do things with the buttons. When debounce matters, checkstatus of timePassed[i] first
    
    if (timePassed[BT_CANCEL]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_CANCEL) && bitRead(buttonState, BT_CANCEL)) {
            buttonCancel();
        }
    }
    
    if (timePassed[BT_UP]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_UP) && bitRead(buttonState, BT_UP)) {
            buttonMenuDown();
        }
    }
    
    if (timePassed[BT_DOWN]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_DOWN) && bitRead(buttonState, BT_DOWN)) {
            buttonMenuUp();
        }
    }

    if (timePassed[BT_RIGHT]) {
        // if pressed but wasn't before
        if (!bitRead(newButtonState, BT_RIGHT) && bitRead(buttonState, BT_RIGHT)) {
            buttonItemRight();
        }
    }

    if (timePassed[BT_LEFT]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_LEFT) && bitRead(buttonState, BT_LEFT)) {
            buttonItemLeft();
        }
    }

    if (timePassed[BT_MEASURE]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_MEASURE) && bitRead(buttonState, BT_MEASURE)) {
            buttonMeasure();
        }
    }

    if (timePassed[BT_SHOOT]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_SHOOT) && bitRead(buttonState, BT_SHOOT)) {
            buttonShoot();
        }
    }

    buttonState = newButtonState; 
}

// Shorten a string
String shortenString(String text, byte length) {
    text.remove(length);
    return text;
}

