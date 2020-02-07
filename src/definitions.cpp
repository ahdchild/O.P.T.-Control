#include "definitions.h"

// Externs definitions
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
PCF857x buttonController(0x20, &Wire);
byte temperature = 0;
byte humidity = 0;


volatile bool PCFInterruptFlag = false;

VL53L1X laserDistance;

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
    
    //Make speed of sound more accurate
    if (accurate) {
        unsigned int currentTime = millis();
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