#ifndef ACTIONS_H
#define ACTIONS_H

#include "definitions.h"


// Functions - Physical actions
    void flash(); // Trigger the connected flash
    
    void shutterOpen(bool hold=false); // Trigger the shutter. If @param hold is set true, must use shutterClose() to close shutter
    void shutterClose(); // Close the shutter

    void takeShot(); // trigger flash, shutter, or lockup depending on settings

    void createWaterdrop(int milliseconds);

    void lightsOff(bool screen = false);
    void lightsOn();

// Shoot actions
void shootDistance();
void cancelShoot();

//Functions - Sensors
    void getTemp(); // Get current temp & humidity. This is currently local to definitions.cpp
    double getUSDistance(bool accurate = true); // return ultrasonic distance. Uses temperature if @param accurate = true
    double getUSDistanceAVG(bool accurate = true); // take multiple readings and return average ultrasonic distance
    double getDistance(bool average = false);

    void buttonShoot();

#endif