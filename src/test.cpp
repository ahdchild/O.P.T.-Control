#include "definitions.h"


int lastLight;

void testSetup() 
{
    //lcd.print("Nothing to see here"); 
    lastLight = analogRead(LIGHT_SENSOR);
    

}

int soundHigh = 0;


void testLoop() 
{


    unsigned int distance = laserDistance.read();
    // if (distance > 450 && distance < 600) flash();
    int soundlevel = analogRead(SOUND_SENSOR);
    // if (soundlevel > 140) flash();
    if (soundlevel > soundHigh) soundHigh = soundlevel;
    lcd.home();
    lcd.print(padInt(soundlevel, 3));
    lcd.setCursor(5,0);
    lcd.print(padInt(soundHigh, 3));
    
    lcd.setCursor(0,1);
    lcd.print((double)distance/10);
    if (laserDistance.timeoutOccurred()) { lcd.print(" TIMEOUT");
    } else lcd.print("        ");
    
    lcd.setCursor(0,2);
    int light = analogRead(LIGHT_SENSOR);
    String lightStr = padInt(light, 3);
    //if (light < 100) lcd.print(" ");
    //if (light < 10) lcd.print(" ");
    lcd.print(lightStr);
    //if (lastLight - light > 100) flash();
    lastLight = light;
}

/*
[x] LCD
[x] Buttons/8574
[x] Ultrasonic Sensor
[x] Temperature Sensor
[x] Laser Distance Sensor
[x] Sound Sensor
[x] Light Sensor
[x] Room Light
[-] Flash Connection - feature removed
[x] Flash Trigger
[-] Camera Connection - feature removed
[x] Camera Shutter
[x] Camera Focus
[x] Solenoid/Waterdrop


*/