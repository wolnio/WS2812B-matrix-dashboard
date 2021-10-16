//code by bitluni. It's public domain but I'm happy about shout outs nevertheless :-)
#include <Arduino.h>

#pragma once
const int MAX_CONTROLLERS = 4;
int controller_button = 0;

class GameControllers
{
public:
    enum Type
    {
        NES = 0,
        SNES = 1,
    };

    enum Button
    {
        B = 0,
        Y = 1,
        SELECT = 2,
        START = 3,
        UP = 4,
        DOWN = 5,
        LEFT = 6,
        RIGHT = 7,
        A = 8,
        X = 9,
        L = 10,
        R = 11,
    };

    Type types[MAX_CONTROLLERS];
    int latchPin;
    int clockPin;
    int dataPins[MAX_CONTROLLERS];
    long buttons[MAX_CONTROLLERS][12];

    ///This has to be initialized once for the shared pins latch and clock
    void init(int latch, int clock)
    {
        latchPin = latch;
        clockPin = clock;
        pinMode(latchPin, OUTPUT);
        digitalWrite(latchPin, LOW);
        pinMode(clockPin, OUTPUT);
        digitalWrite(clockPin, HIGH);
        for (int c = 0; c < MAX_CONTROLLERS; c++)
        {
            for (int i = 0; i < 12; i++)
                buttons[c][i] = -1;
            types[c] = NES;
            dataPins[c] = -1;
        }
    }

    ///This sets the controller type and initializes its individual data pin
    void setController(int controller, Type type, int dataPin)
    {
        types[controller] = type;
        dataPins[controller] = dataPin;
        pinMode(dataPins[controller], INPUT_PULLUP);
    }

    void poll()
    {
        digitalWrite(latchPin, HIGH);
        delayMicroseconds(12);
        digitalWrite(latchPin, LOW);
        delayMicroseconds(6);
        for (int i = 0; i < 12; i++)
        {
            for (int c = 0; c < MAX_CONTROLLERS; c++)
                if (dataPins[c] > -1)
                {
                    if (digitalRead(dataPins[c]))
                        buttons[c][i] = -1;
                    else
                        buttons[c][i]++;
                }
            digitalWrite(clockPin, LOW);
            delayMicroseconds(6);
            digitalWrite(clockPin, HIGH);
            delayMicroseconds(6);
        }
    }

    int translate(int controller, Button b) const
    {
        if (types[controller] == SNES)
            return b;
        static const int translateToNES[] = {1, 8, 2, 3, 4, 5, 6, 7, 0, 8, 8, 8};
        return translateToNES[b];
    }

    ///button will be unpressed until released again
    void clear(int controller, Button b)
    {
        buttons[controller][translate(controller, b)] = 0x80000000;
    }

    ///returns if button is currently down
    bool down(int controller, Button b) const
    {
        return buttons[controller][translate(controller, b)] >= 0;
    }

    ///returns true if button state changed to down since previous poll. repeatAfterTics can be used to repeat after button was hold down for sme time
    bool pressed(int controller, Button b, long repeatAfterTics = 0x7fffffff) const
    {
        return buttons[controller][translate(controller, b)] == 0 || (buttons[controller][translate(controller, b)] >= repeatAfterTics);
    }

    ///returns the type of controller configured
    Type getType(int controller)
    {
        return types[controller];
    }
};

//shared pins between all controllers
const int LATCH_PIN = 19;
const int CLOCK_PIN = 18;
//individual data pin for each controller
const int DATA_PIN_0 = 21;

GameControllers controllers;

void readNES()
{
    controllers.poll();                                       //read all controllers at once
    if (controllers.pressed(0, GameControllers::START, 100))  //check if Start was pressed since last loop
        controller_button = 1;                                //START
    if (controllers.pressed(0, GameControllers::SELECT, 100)) //check if Select was pressed since last loop
        controller_button = 2;                                //SELECT
    if (controllers.pressed(0, GameControllers::A, 100))      //if A button is hold down repeat after 20 loops
        controller_button = 3;                                //A
    if (controllers.pressed(0, GameControllers::B, 100))      //check if B button it's currently pressed down
        controller_button = 4;                                //B
    if (controllers.pressed(0, GameControllers::UP, 100))     //if UP button is hold down repeat after 20 loops
        controller_button = 5;                                //UP
    if (controllers.pressed(0, GameControllers::DOWN, 100))   //if DOWN button is hold down repeat after 20 loops
        controller_button = 6;                                //DOWN
    if (controllers.pressed(0, GameControllers::RIGHT, 100))  //if RIGHT button is hold down repeat after 20 loops
        controller_button = 7;                                //RIGHT
    if (controllers.pressed(0, GameControllers::LEFT, 100))   //if LEFT button is hold down repeat after 20 loops
        controller_button = 8;                                //LEFT
    
    delay(100);                                               //read controller just ~20times a second
}