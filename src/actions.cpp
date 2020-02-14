#include  "definitions.h"

byte temperature = 0;
byte humidity = 0;

//Track when temperature was updated
unsigned int lastTempUpdate = 0;

//Track when flash and a shot was last fired
unsigned long lastFlash = 0;
unsigned long lastShot = 0;

// FUNCTIONS
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

void createWaterdrop(int milliseconds) {
    digitalWrite(WATERDROP, HIGH);
    delay(milliseconds);
    digitalWrite(WATERDROP, LOW);
}

void lightsOn() {
    if (SettingSettings[SET_LIGHT] != OFF) {
        lcd.backlight();
        digitalWrite(ROOM_LIGHT, HIGH);
    }
}

void lightsOff(bool screen) {
    if (SettingSettings[SET_LIGHT] != ON) {
        if (screen) lcd.noBacklight();
        digitalWrite(ROOM_LIGHT, LOW);
    }
  }

double getDistance(bool average) {
    double distance;
    if (distanceSettings[DM_SENSOR] == USE_LASER)
        distance = laserDistance.read()/10;
    else if (average) distance = getUSDistanceAVG();
    else distance = getUSDistance();

    return distance;
}

void takeShot() {
    
    flash();
}

void buttonShoot() {
    if (mode != STANDBY_MODE) return;

    if (currentMenu == DistanceMenu) {
        getTemp();
        lightsOff();
        if (distanceSettings[DM_WHEN] == ON_CHANGE)
            distanceSettings[DM_DISTANCE] = getDistance();
        mode = DISTANCE_MODE;
        lcd.clear();
    }
}

void shootDistance() {
    if (millis() - lastShot < RestBetweenShots) return;

    double distance = getDistance();
    bool shoot = false;

    lcd.home();
    lcd.print("Distance = ");
    lcd.print(distance);
    lcd.print("cm    ");


    if (distanceSettings[DM_WHEN] == GREATER_THAN && distance > distanceSettings[DM_DISTANCE])
        shoot = true;
    else if (distanceSettings[DM_WHEN] == LESS_THAN && distance < distanceSettings[DM_DISTANCE])
        shoot = true;
    else if (distanceSettings[DM_WHEN] == AROUND) {
        if (distanceSettings[DM_DISTANCE] - distanceSettings[DM_RANGE] < distance && distance < distanceSettings[DM_DISTANCE] + distanceSettings[DM_RANGE] )
            shoot = true;
    } else {
        if (distance < distanceSettings[DM_DISTANCE] - distanceSettings[DM_RANGE] || distance > distanceSettings[DM_DISTANCE] + distanceSettings[DM_RANGE] )
            shoot = true;
    }

    if (shoot) {
        if (distanceSettings[DM_DELAY]>0) delay(distanceSettings[DM_DELAY]);
        takeShot();
        lastShot = millis();
        if (!distanceSettings[DM_CONTINUOUS]) mode = CANCELLING_MODE;
    }
}


void cancelShoot() {
        lcd.clear();
        lcd.print("cancelling");
        
        mode = STANDBY_MODE;
        shutterClose();
        lightsOn();

        screenReprintNeeded = true;
}