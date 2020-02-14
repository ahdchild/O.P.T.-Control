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

    // reprint the screen if needed
    PrintScreen();
}