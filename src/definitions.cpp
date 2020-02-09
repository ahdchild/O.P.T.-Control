#include "definitions.h"

// Externs definitions
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
PCF857x buttonController(0x20, &Wire);
byte temperature = 0;
byte humidity = 0;
byte buttonState = B11111111;
byte mode = STANDBY_MODE;


volatile bool PCFInterruptFlag = false;
VL53L1X laserDistance;
long lastDebounceTime[] = {0,0,0,0,0,0,0,0};
const long debounceDelay = 10;

// Non Extern Variables
SimpleDHT11 tempSensor(TEMPERATURE);
unsigned long lastFlash = 0;
unsigned int lastTempUpdate = millis();

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
    //laserDistance.init();
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
}

// Handle button interrupts
void PCFInterrupt() {
  PCFInterruptFlag = true;
}

void flash() {
    unsigned int currentTime = millis();

    if (currentTime - lastFlash > flashCooldown) {  
        lastFlash = currentTime;
        digitalWrite(FLASH_TRIGGER, HIGH);
        delay(10);
    }
    digitalWrite(FLASH_TRIGGER, LOW);
}

void shutterOpen(bool hold) {
    digitalWrite(CAMERA_SHUTTER, HIGH);
    if (!hold) {
        delay(100);
        shutterClose();
    }
}

void shutterClose() {
    digitalWrite(CAMERA_SHUTTER, LOW);
}

void getTemp() {
    byte maxTries = TempRetries;
    int err;
    do{
        err = tempSensor.read(&temperature, &humidity, NULL);
        maxTries--;
    } while (err != SimpleDHTErrSuccess && maxTries > 0);
}

double getUSDistance(bool accurate) {
    long duration;
    double distance = 0;
    float speedofsound = 340;

    // Clear the trigPin by setting it LOW:
    digitalWrite(US_TRIGGER, LOW);
    delayMicroseconds(5);

    // Trigger the sensor by setting the trigPin high for 10 microseconds:
    digitalWrite(US_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_TRIGGER, LOW);

    // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
    duration = pulseIn(US_ECHO, HIGH);
    
    // Use current temperature tyo calculate more accuarate speed of sound
    if (accurate) {
        unsigned int currentTime = millis();
        // update temp if it hasn't been updated recently
        if (currentTime-lastTempUpdate>TempUpdateFreq) {
            getTemp();
            lastTempUpdate = currentTime;
        }
        speedofsound = 331.3+(0.606*temperature);
    }
    // Calculate the distance:
    distance= duration*(speedofsound/10000)/2;

    return distance;
}

double getUSDistanceAVG(bool accurate) {
    double distance = 0;
    for (int i = 0; i<USDistanceAVG; i++){
        distance += getUSDistance(accurate);
    }
    distance = distance/USDistanceAVG;
    return distance;
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
    if (!PCFInterruptFlag) return;

    PCFInterruptFlag = false;
    bool timePassed[8];

    byte newButtonState = buttonController.read8();

    // Check each button for debounce delay.
    // timePassed[i] is set true if the debounbce time has passed
    for (int i = 0; i < 8; i++) {
        timePassed[i] = ((millis() - lastDebounceTime[i]) > debounceDelay);
        if (timePassed[i]) lastDebounceTime[i] = millis();
    }

    // do things with the buttons. When debounce matters, checkstatus of timePassed[i] first
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
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_RIGHT) && bitRead(buttonState, BT_RIGHT)) {
            buttonItemLeft();
        }
    }

    if (timePassed[BT_LEFT]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_LEFT) && bitRead(buttonState, BT_LEFT)) {
            buttonItemRight();
        }
    }

    if (timePassed[BT_CANCEL]) {
        // if it's pressed but wasn't before
        if (!bitRead(newButtonState, BT_CANCEL) && bitRead(buttonState, BT_CANCEL)) {
            buttonCancel();
        }
    }


    buttonState = newButtonState; 
}

// Shorten a string
String shortenString(String text, byte length) {
    text.remove(length);
    return text;
}