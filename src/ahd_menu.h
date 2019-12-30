#ifndef AHD_MENU_H
#define AHD_MENU_H

#include <Arduino.h>

#define MAX_TEXT_OPTIONS 3 //maximum number of options for STRING type menuitem
#define MAX_MENU_ITEMS 6 //maximum number of items in a  menu

//types for menuItem.type
#define NUMBER 0
#define SUBMENU 1
#define STRING 2 
#define COMMAND 3

class menuitem {
    public:
        menuitem(String name="???", int minimum=0, int maximum=100, int defaultValue = 0); //Constructor initializes a NUMBER type menuItem.
        menuitem(String name, String choices[], byte numberOfChoices); //Constructor initializes a STRING type menu item
        String printValue(); //returns textOptions[value] if type is string. Returns value if type is integer
        String printItem(); //returns label and printValue() formatted as a printable menu item
        void buttonDown(); //handler for down buttonpress
        void buttonUp(); //handler for up button press
        void buttonOK(); //handler for ok or enter button press. May not use this as menu class will have a similar handler.
        int setValue(int x); //set value to x. Returns 0 normally. If x is above max, value will be set to max and 1 will be returned. If x is below min, min will be set and -1 will be returned
        int getValue(); //return value
        void setLabel(String name); //set the label of this item to name
        String getLabel(); //retuns the label for this item
        void setType(int x); //sets the type of this item. 0=integer, 1=string, 2=submenu, 3=action command
        int getType(); //returns type of this item
        void setMin(int x); //sets the minimum allowed for value
        int getMin(); //returns the minimum allowed for value
        void setMax(int x); //sets the maximum allowed for value
        int getMax(); //returns the maximum allowed for value
        bool addTextOption(String newText, int x); //add a string to the list of menu options at index x. Returns false if it fails
        String getTextOption(int x); //return textOption[x]. If there is no entry, return empty string

    private:
        String label; //name of the menu item
        int value; //value of the menu item (or actionID if this is an action command type item)
        byte type; //Type of menu item. 0=integer, 1=string, 2=submenu, 3=action command
        int min; //minimum allowable value
        int max; //maximum allowable value
        String *textOptions; //text for menu options. Length should match max. min should be set to 0 to correspond with 1st index
};

class menu {
    public:
        menu();
        menu(String name, menuitem itemList[], byte listSize, byte screenRows = 2);
        void buttonDown(); //handler for down buttonpress
        void buttonUp(); //handler for up button press
        void buttonOK(); //handler for ok or enter button press
        void AddMenuItem(const menuitem &newItem, int x); //add an item to the menu
        int screenSelection(); //returns which line on screen is selected. Counting from 0
        String printLine(int x); //returns line to be printed on lcd row x. Use with lcd.print()
        menuitem& item(int x =-1); // allows direct access to the menuitem at index x. if x unset, returns currently selected item

    private:
        String title;
        byte actionID; //identifier of action to be taken if action buttoin is pressed from this menu
        byte itemCount; //number of menu items on this menu. Remember to subtract 1 when counting from 0
        byte currentPage; //index of the first iten from items[] currently being displayed
        byte currentItem; //index of the currently selected item/line 
        byte linesPerPage; //how many lines will fit on a page. Determined by display being used
        menuitem *items; //a list of menu items to show in this menu
        menuitem nullItem; // for returning NULL
};

#endif