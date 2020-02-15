#include "definitions.h"

void setup() {
    //Initialize stuff
    initializeStuff();

    // run tests
    //testSetup();
}


void loop() {
    // testLoop();

    // handle button presses
    handleButton();

    // take actions for current mode
    if (mode == CANCELLING_MODE) cancelShoot();
    else if (mode == DISTANCE_MODE) shootDistance();
    else if (mode == SOUND_MODE) shootSound();
    else if (mode == WATER_MODE) shootWaterdrop();
    else if (mode == LIGHTNING_MODE) shootLightning();
    else if (mode == INTERVAL_MODE) shootIntervalometer();

    // reprint the screen if needed
    PrintScreen();
}