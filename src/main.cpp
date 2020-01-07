#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


#include "ahd_camera.h"
#include "ahd_menu.h"
#include "mainmenu.h"

//Pin assignments
const int FLASH = 4; // flash trigger
const int SHUTTER = 7; // camera shutter trigger
const int LIGHT = 3; // LED to light room
const int JOYSTICK_X = 0;
const int JOYSTICK_Y = 1;
const int JOYSTICK_BUTTON = 13;

int joystickXCenter, joystickYCenter;

//Timers
int timer1;

//16x2 LCD variables
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Additional variables
bool controlMenu = true;
bool wasCentered = true;

//Function Declarations
void simpleTrigger(int triggerPin=FLASH, int duration=50); //Trigger the flash or shutter
void lockupTrigger(int lockupPin=SHUTTER, int triggerPin=FLASH, int triggerDuration=50, int lockupDelay=1000); //open the shutter, trigger the flash, close the shutter
void rotaryTurn();
void refreshMenuScreen();
void runAction(byte actionID);



//***Setup
void setup() {

  //Pin Modes
  pinMode(FLASH, OUTPUT);
  pinMode(SHUTTER, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(JOYSTICK_BUTTON, INPUT);
  digitalWrite(JOYSTICK_BUTTON, HIGH);

  //Calibrate Joystick
  joystickXCenter = analogRead(JOYSTICK_X);
  joystickYCenter = analogRead(JOYSTICK_Y);

  //Set runAction to handle actions from the menus
  currentMenu->setActionHandler(runAction);
  
  //Initialize 16x2 LCD
  lcd.init();
  lcd.backlight();
 
  lcd.clear();

  //Display menu
  refreshMenuScreen();
   lcd.print("test"); 

 
}




//Loop
void loop() {
// int currentTime = millis(); //for any timers

  
  // handle button presses
  if (!digitalRead(JOYSTICK_BUTTON)) {
    delay(400);
    if (currentMenu->item().getType() == MENU_SUBMENU || currentMenu->item().getType() == MENU_COMMAND) {
      currentMenu->buttonOK();
    } else controlMenu = !controlMenu;
    refreshMenuScreen();
  }


   // handle rotary turns
  int joyYInput = analogRead(JOYSTICK_Y) - joystickYCenter;
  int joyXInput = analogRead(JOYSTICK_X) - joystickXCenter;
  if (abs(joyYInput) > 100 && wasCentered) {
    wasCentered = false;
    if (joyYInput < 0) {
      if (!controlMenu) currentMenu->item().buttonUp();
      else currentMenu->buttonDown();
    } else {
      if (!controlMenu) currentMenu->item().buttonDown();
      else currentMenu->buttonUp();
    }
    refreshMenuScreen();
  } /* else if (abs(joyXInput) > 100 && wasCentered) {
    if (joyXInput > 0) {
      if (!controlMenu) currentMenu->item().buttonUp();
      else currentMenu->buttonDown();
    } else {
      if (!controlMenu) currentMenu->item().buttonDown();
      else currentMenu->buttonUp();
    }
    wasCentered = false;
    refreshMenuScreen();
  } */ 

  if (abs(joyYInput) < 10) wasCentered = true;
}


//refreshMenuScreen()
//reprints the menu on the lcd
void refreshMenuScreen() {
  //before printing, make sure currentMenu is correct  
  currentMenu = &menuArray[currentMenu->currentMenu];
  
  lcd.clear();
  for (int i = 1; i <= SCREEN_SIZE; i++) {
    lcd.setCursor(0,i-1);
    lcd.print(currentMenu->printLine(i));

    //add an indicator if controlling an item value
    //this can be removed when more buttons are added
    if (!controlMenu) {
      lcd.setCursor(15,currentMenu->screenSelection());
      lcd.print("<");
    }
  }
}


//Trigger the flash or shutter
void simpleTrigger(int triggerPin, int duration) {
  digitalWrite(triggerPin, HIGH);
  delay(duration);
  digitalWrite(triggerPin, LOW);
}


//open the shutter, trigger the flash, close the shutter
void lockupTrigger(int lockupPin, int triggerPin, int triggerDuration, int lockupDelay) {
    digitalWrite(lockupPin, HIGH);
    delay(lockupDelay);
    simpleTrigger(triggerPin, triggerDuration);
    delay(lockupDelay);
    digitalWrite(lockupPin, LOW);
}


void runAction(byte actionID) {
  lcd.clear();
  lcd.print("Running action ");
  lcd.setCursor(0,1);
  lcd.print(actionID);
  delay(2000);
}