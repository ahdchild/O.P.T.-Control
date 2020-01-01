#include <Arduino.h>
#include <LiquidCrystal.h>
#include "ahd_camera.h"
#include "ahd_menu.h"
#include "mainmenu.h"

//Pin assignments
const int FLASH = 4; // flash trigger
const int SHUTTER = 7; // camera shutter trigger
const int LIGHT = 3; // LED to light room
const int BUTTON = 13; //Rotary button
const int ROT_CLK = 2; //Rotary CLK pin
const int ROT_DT = 1; //Rotary DT pin

//Rotary encoder variables
volatile boolean TurnDetected = false;  // True if a rotary turn happened. Need volatile for Interrupts
volatile int rotationDirection;  // 1 for right turn, -1 for left turn
volatile int lastRotation = millis(); // last time a rotary turn happened for debouncing

//Timers
int currentTime = millis();
int timer1;

//16x2 LCD variables
const int rs = 8, en = 12, d4 = 0, d5 = 5, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Additional variables
bool controlMenu = true;

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
  pinMode(BUTTON, INPUT);

  //pullup for the rotary button
  digitalWrite(BUTTON, HIGH);

  //Interrupt 0 connected to pin 2 on Arduino UNO
  attachInterrupt (0, rotaryTurn, FALLING);
  
  //Set runAction to handle actions from the menus
  currentMenu->setActionHandler(runAction);

  //Initialize 16x2 LCD
  lcd.begin(16, 2); 
  lcd.clear();
  refreshMenuScreen();
}




//Loop
void loop() {
  currentTime = millis(); //for any timers

  // handle button presses
  if (!digitalRead(BUTTON)) {
    delay(400);
    if (currentMenu->item().getType() == SUBMENU || currentMenu->item().getType() == COMMAND) {
      currentMenu->buttonOK();
    } else controlMenu = !controlMenu;
    refreshMenuScreen();
  }

  // handle rotary turns
  if (TurnDetected) {
    lcd.clear();
    if (rotationDirection == 1) {
      if (!controlMenu) currentMenu->item().buttonUp();
      else currentMenu->buttonDown();
    } else {
      if (!controlMenu) currentMenu->item().buttonDown();
      else currentMenu->buttonUp();
    }
    refreshMenuScreen();
    TurnDetected = false;
  }  
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


void rotaryTurn() {
  //If ROT_DT and ROT_CLK are both high or both low, it's spinning forward. If they're different, it's going backward.
  if (digitalRead(ROT_DT) == digitalRead(ROT_CLK)) {
    rotationDirection = 1;  
  } else rotationDirection = -1;


  //Debounce - throw away changes within 100 miliseconds
  if (currentTime - lastRotation > 100) {
    TurnDetected = true;
    lastRotation = currentTime;
  }
}

void runAction(byte actionID) {
  lcd.clear();
  lcd.print("Running action ");
  lcd.setCursor(0,1);
  lcd.print(actionID);
  delay(2000);
}