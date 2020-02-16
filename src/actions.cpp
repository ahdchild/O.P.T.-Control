#include  "definitions.h"

byte temperature = 0;
byte humidity = 0;
int lastLightReading;

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

void lockup() {
    shutterOpen(true);
    delay(150);
    flash();
    delay(10);
    shutterClose();
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
    lcd.backlight();

    if (SettingSettings[SET_LIGHT] != OFF) {
        digitalWrite(ROOM_LIGHT, HIGH);
    }
}

void lightsOff(bool screen) {
    if (SettingSettings[SET_LIGHT] != ON) {
        digitalWrite(ROOM_LIGHT, LOW);
    }
    if (screen && SettingSettings[SET_SCREEN] == AUTO) lcd.noBacklight();
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

    switch (SettingSettings[SET_TRIGGER])
    {
    case FIRE_FLASH:
        flash();
        break;
    case FIRE_SHUTTER:
        shutterOpen();
        break;
    case LOCKUP:
        flash();
        delay(10);
        shutterClose();
        break;
    
    default:
        break;
    }
}

void buttonShoot() {
    if (mode != STANDBY_MODE) return;

    switch (currentMenu)
    {
        case DistanceMenu:
            getTemp();
            lightsOff(true);
            if (distanceSettings[DM_WHEN] == ON_CHANGE)
                distanceSettings[DM_DISTANCE] = getDistance();
            mode = DISTANCE_MODE;
            break;
        case SoundMenu:
            lightsOff(true);
            mode = SOUND_MODE;
            break;
        case LightningMenu:
            // lightsOff(true);
            mode = LIGHTNING_MODE;
            lastLightReading = analogRead(LIGHT_SENSOR);
            break;
    }

    
    if (mode > CANCELLING_MODE) {
        lcd.clear();  
        if (SettingSettings[SET_TRIGGER] == LOCKUP) {
            shutterOpen(true);
            delay(150);
        }
    }

}

void shootDistance() {
    if (millis() - lastShot < RestBetweenShots) return;

    double distance = getDistance();
    bool shoot = false;

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
        else if (SettingSettings[SET_TRIGGER] == LOCKUP) {
            shutterOpen(true);
            delay(150);
        }
        
    }
}

void shootSound() {
    if (millis() - lastShot < RestBetweenShots) return;

    bool shoot = false;
    int noise = analogRead(SOUND_SENSOR);

    if (noise > soundSettings[SO_THRESHOLD]) shoot = true;

    if (shoot) {
        if (soundSettings[SO_DELAY]>0) delay(soundSettings[SO_DELAY]);
        takeShot();
        lastShot = millis();
        if (!distanceSettings[DM_CONTINUOUS]) mode = CANCELLING_MODE;
        else if (SettingSettings[SET_TRIGGER] == LOCKUP) {
            shutterOpen(true);
            delay(150);
        }
    }
}

void shootWaterdrop() {

}

void shootLightning() {
    if (millis() - lastShot < RestBetweenShots) return;

    bool shoot = false;
    int currentLight = analogRead(LIGHT_SENSOR);

    if (lastLightReading - currentLight > lightningSettings[LI_CHANGE_AMT]) shoot = true;

    lastLightReading = currentLight;

    if (shoot) {
        shutterOpen();
        lastShot = millis();
        if (!distanceSettings[DM_CONTINUOUS]) mode = CANCELLING_MODE;

        lcd.setCursor(0,1);
        lcd.print(padInt(lastLightReading - currentLight, 3));
    }
}

void shootIntervalometer() {

}

void cancelShoot() {
        lcd.clear();
        
        mode = STANDBY_MODE;
        shutterClose();
        lightsOn();

        screenReprintNeeded = true;
}