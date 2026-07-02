/*
==========================================================
 buttons.h
----------------------------------------------------------
 Button Input Module Interface

 Responsibilities
    - Button initialization
    - Debouncing
    - Button state management
    - Navigation events
    - Future input expansion

 NOTE:
    This module provides high-level button events.
    The rest of the application should never read
    digital pins directly.
==========================================================
*/

#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>



//----------------------------------------------------------
// Button Enumeration
//----------------------------------------------------------

enum ButtonID
{
    BUTTON_PREVIOUS,
    BUTTON_NEXT,
    BUTTON_UNKNOWN
};



//----------------------------------------------------------
// Button State
//----------------------------------------------------------

struct ButtonState
{
    bool current;
    bool previous;

    bool pressed;
    bool released;

    unsigned long pressedTime;
};



//----------------------------------------------------------
// Initialization
//----------------------------------------------------------

/*
    Configure all button pins.

    Call once from setup().
*/
void initButtons();



//----------------------------------------------------------
// Update
//----------------------------------------------------------

/*
    Reads all buttons.

    Call once every loop().
*/
void updateButtons();



//----------------------------------------------------------
// Navigation Events
//----------------------------------------------------------

/*
    Returns true once when the
    Previous button is pressed.
*/
bool previousPagePressed();

/*
    Returns true once when the
    Next button is pressed.
*/
bool nextPagePressed();



//----------------------------------------------------------
// Generic Button Queries
//----------------------------------------------------------

bool buttonPressed(ButtonID button);

bool buttonReleased(ButtonID button);

bool buttonHeld(ButtonID button);



//----------------------------------------------------------
// Future Features
//----------------------------------------------------------

/*
    Reserved for future expansion.

    Suggested implementations:

    - Long press
    - Double click
    - Triple click
    - Auto-repeat
*/

bool longPress(ButtonID button);

bool doubleClick(ButtonID button);

bool tripleClick(ButtonID button);



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

/*
    Prints button information
    to the Serial Monitor.
*/
void printButtonReport();

#endif
