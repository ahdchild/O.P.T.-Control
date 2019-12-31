#include <Arduino.h>
#include <Assert.h>
#include "ahd_menu.h"



// CLASS - menuitem

// Constructor initializes a NUMBER type menuItem.
menuitem::menuitem(const String &name, int minimum, int maximum, int defaultValue)
    : label {name}
    , value {defaultValue}
    , type {NUMBER} 
    , min {minimum}
    , max {maximum}
    , textOptions {nullptr}
{
        if (max < min) max = min; //make sure max is not less than min. Throw an error instead when I figure out how
        if (value < min) value = min; //fix value if it's out of range
        if (value > max) value = max;
}

//Constructor initializes a String type menu item
menuitem::menuitem(const String &name, String choices[], byte numberOfChoices) 
     : label {name}
    , value {0}
    , type {STRING} 
    , min {0}
    , max {numberOfChoices-1}
    , textOptions {choices}
{
    //textOptions = choices;
}

 //Constructor initializes a SUBMENU type menu item
 menuitem::menuitem(byte targetIndex, const String &targetName)
     : label {targetName}
    , value {targetIndex}
    , type {SUBMENU} 
    , min {targetIndex}
    , max {targetIndex}
    , textOptions {nullptr}
 {

 }

//returns textOptions[value] if type is string. Returns value if type is integer
String menuitem::printValue() {
    String returnString = String(value);
    if (type == STRING) returnString = textOptions[value];
    return returnString;
}

//returns label and printValue() formatted as a printable menu item
String menuitem::printItem() {
    String returnString;
    if (type == NUMBER || type == STRING) {
        returnString = String(label + ": ");
        returnString = String(returnString + printValue());
    } else if (type == SUBMENU)returnString = String(label+" ->");

    return returnString;

}

//set value to x. Returns 0 normally. If x is above max, value will be set to max and 1 will be returned. If x is below min, min will be set and -1 will be returned
int menuitem::setValue(int x) {
    if (x < min) {
        return -1; // fail if value is below min
    } else if (x > max) {
        return 1; // fail if value is above max
    } else if ( type == NUMBER || type == STRING) value = x; // set value if it's within range
    return 0;
}

//return value
int menuitem::getValue() {
    return value;
}

//handler for down buttonpress
void menuitem::buttonDown() {
    switch (type) {
        case SUBMENU:
            break;
        case COMMAND:
            break;
        default: setValue(value - 1);
    }
}

//handler for up button press
void menuitem::buttonUp() {
    // submenu and command need implementation
    switch (type) {
        case SUBMENU:
            break;
        case COMMAND:
            break;
        default: setValue(value + 1);
    }
}

//set the label of this item to name
/*
void menuitem::setLabel(const String &name) {
    // maybe add some length checking
    label = name;
} */

//retuns the label for this item
String menuitem::getLabel() {
    return label;
}

//add a string to the list of menu options at index x. Returns false if it fails
/*
bool menuitem::addTextOption(const String &newText, int x) {
    if (x >= 0 && x <= max && x <= MAX_TEXT_OPTIONS - 1) { // make sure x is within range
        textOptions[x] = newText;
        return true; // Success
    } else return false; // failure
} */

//sets the type of this item. 0=integer, 1=string, 2=submenu, 3=action command
/*
void menuitem::setType(int x) {
    if (x < NUMBER || x > COMMAND) return; //Do nothing if x is not a valid type
    
    //Extra setup if type is being changed to string
    if (x == STRING && type != STRING) {
        min = 0; // value for STRING menu items should start at 0
        if (max > MAX_TEXT_OPTIONS - 1) max = MAX_TEXT_OPTIONS - 1;
        value = 0;
        //populate textOptions[]
        for (int i = 0; i <= max; i++) {
            textOptions[i] = String("??? " + String(i));
        }
    }

    type = x;
}*/

//returns type of this item
int menuitem::getType() {
    return type;
}


//sets the minimum allowed for value
/*
void menuitem::setMin(int x) {
    if (type==NUMBER) min = x; //only NUMBER types should have changeable min
    if (min > max) max = min; //make sure max is not less than min
}*/

//returns the minimum allowed for value
int menuitem::getMin() {
    return min;
}

//sets the maximum allowed for value
/*
void menuitem::setMax(int x) {
    //if this is a string, do nothing if out of range
    if (type == STRING && (x < 0 || x > MAX_TEXT_OPTIONS - 1)) return;
    max = x;
    if (min > max) min = max; //make sure max is not less than min
}
*/

//returns the maximum allowed for value
int menuitem::getMax() {
    return max;
}

//return textOption[x]. If there is no entry, return empty string
String menuitem::getTextOption(int x) {
    //Return nothing if type is not string or x is out of range
    if (type != STRING || x < min || x > max) return "";
    return textOptions[x];
} 

// void buttonOK(); //handler for ok or enter button press. May not use this as menu class will have a similar handler.
        



// CLASS - menu

byte menu::currentMenu = 0;

menu::menu ()
    : title {""}
    , actionID {0}
    , itemCount {0}
    , currentPage {0}
    , currentItem {0}
    , linesPerPage {0}
    , items {nullptr}
    , menuIndex {0}
{
}

menu::menu(const String &name, menuitem itemList[],  byte listSize, byte index, byte screenRows) 
    : title {name}
    , actionID {0}
    , itemCount {listSize}
    , currentPage {0}
    , currentItem {0}
    , linesPerPage {screenRows}
    , menuIndex {index}
{
    items = itemList;
}

//add an item to the menu
/*void menu::AddMenuItem(const menuitem &newItem, int x) {
    if (x >= 0 and x <= itemCount) {
        items[x] = newItem;
    }
} */

//handler for down buttonpress
void menu::buttonDown() {
    if (currentItem == itemCount - 1) return; //do nothing if already at end of list
    currentItem++; //select the next item
    if (currentItem >= currentPage + linesPerPage) currentPage = currentItem - (linesPerPage-1); //Scroll the page if needed
}

 //handler for up button press
void menu::buttonUp() {
    if (currentItem == 0) return; //do nothing if already at start of list
    currentItem--; //select the previous item
    if (currentItem < currentPage) currentPage = currentItem; //Scroll the page if needed
}

//returns line to be printed on lcd row x. Use with lcd.print()
String menu::printLine(int x) {
    int realIndex = currentPage + (x - 1); //x is a screen line. Turn it into index of item to be printed
    if (x < 1  || x > linesPerPage || x > itemCount) return F("");

    String returnValue = " ";//prefix for unselected items
    if (realIndex == currentItem) returnValue = F(">"); //prefix for selected items

    returnValue = String(returnValue + items[realIndex].printItem()); //add the item

    return returnValue;
}

//returns which line on screen is selected. Counting from 0
int menu::screenSelection() {
    return (currentItem - currentPage);
}

// allows direct access to the menuitem at index x
menuitem& menu::item(int x) { 
    if (x == -1) x= currentItem;
    if (x >= 0 and x <= itemCount) {
        return items[x];
    }
    return nullItem;
}

void menu::buttonOK() {
    // if the current item is a SUBMENU type, change currentMenu to index the menu to switch to
    if (item().getType() == SUBMENU) {
        currentMenu = item().getValue();
        if (menuIndex > 0) currentItem = currentPage = 0; //Leave currentItem where it is when leaving the main menu, otherwise reset it
    }
}