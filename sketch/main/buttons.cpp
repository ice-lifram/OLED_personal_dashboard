/*
==========================================================
 buttons.cpp
----------------------------------------------------------
 Button Input Module

 Responsibilities
    - Initialize button pins
    - Read button states
    - Detect edge transitions
    - Basic software debouncing
    - Future button interactions

 NOTE:
    This module should be the ONLY place where
    digitalRead() is used.
==========================================================
*/

#include "buttons.h"
#include "config.h"



//----------------------------------------------------------
// Configuration
//----------------------------------------------------------

static const unsigned long DEBOUNCE_DELAY = 30;



//----------------------------------------------------------
// Private Button States
//----------------------------------------------------------

ButtonState previousButton =
{
    HIGH,
    HIGH,
    false,
    false,
    0
};

ButtonState nextButton =
{
    HIGH,
    HIGH,
    false,
    false,
    0
};



//----------------------------------------------------------
// Private Helper
//----------------------------------------------------------

static void updateButton(
    ButtonState &button,
    uint8_t pin)
{
    bool reading = digitalRead(pin);

    button.pressed  = false;
    button.released = false;

    // Basic edge detection
    if (reading != button.current)
    {
        delay(DEBOUNCE_DELAY);

        reading = digitalRead(pin);

        if (reading != button.current)
        {
            button.previous = button.current;
            button.current  = reading;

            if (button.current == LOW)
            {
                button.pressed = true;
                button.pressedTime = millis();
            }
            else
            {
                button.released = true;
            }
        }
    }
}



//----------------------------------------------------------
// Initialization
//----------------------------------------------------------

void initButtons()
{
    pinMode(PREVIOUS_BUTTON_PIN, INPUT_PULLUP);
    pinMode(NEXT_BUTTON_PIN, INPUT_PULLUP);

    Serial.println(F("Buttons initialized."));
}



//----------------------------------------------------------
// Update
//----------------------------------------------------------

void updateButtons()
{
    updateButton(previousButton, PREVIOUS_BUTTON_PIN);

    updateButton(nextButton, NEXT_BUTTON_PIN);
}



//----------------------------------------------------------
// Navigation Events
//----------------------------------------------------------

bool previousPagePressed()
{
    return previousButton.pressed;
}



bool nextPagePressed()
{
    return nextButton.pressed;
}



//----------------------------------------------------------
// Generic Queries
//----------------------------------------------------------

bool buttonPressed(ButtonID button)
{
    switch (button)
    {
        case BUTTON_PREVIOUS:
            return previousButton.pressed;

        case BUTTON_NEXT:
            return nextButton.pressed;

        default:
            return false;
    }
}



bool buttonReleased(ButtonID button)
{
    switch (button)
    {
        case BUTTON_PREVIOUS:
            return previousButton.released;

        case BUTTON_NEXT:
            return nextButton.released;

        default:
            return false;
    }
}



bool buttonHeld(ButtonID button)
{
    ButtonState* selected = nullptr;

    switch (button)
    {
        case BUTTON_PREVIOUS:
            selected = &previousButton;
            break;

        case BUTTON_NEXT:
            selected = &nextButton;
            break;

        default:
            return false;
    }

    return (selected->current == LOW);
}



//----------------------------------------------------------
// Future Features
//----------------------------------------------------------

bool longPress(ButtonID button)
{
    /*
        TODO

        Suggested implementation:

        if(buttonHeld(button))
        {
            if(millis() - pressedTime > LONG_PRESS_TIME)
                return true;
        }
    */

    (void)button;

    return false;
}



bool doubleClick(ButtonID button)
{
    /*
        TODO

        Store last release time.
        Detect two presses within
        a configurable interval.
    */

    (void)button;

    return false;
}



bool tripleClick(ButtonID button)
{
    /*
        TODO

        Extension of double-click logic.
    */

    (void)button;

    return false;
}



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

void printButtonReport()
{
    Serial.println();

    Serial.println(F("===== BUTTON REPORT ====="));

    Serial.print(F("Previous: "));
    Serial.println(
        previousButton.current == LOW ?
        F("PRESSED") :
        F("RELEASED")
    );

    Serial.print(F("Next: "));
    Serial.println(
        nextButton.current == LOW ?
        F("PRESSED") :
        F("RELEASED")
    );

    Serial.println(F("========================="));
}
