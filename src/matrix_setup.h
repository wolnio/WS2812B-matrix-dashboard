#include <Arduino.h>
#include <FastLED.h>

#pragma once

#define LED_PIN 23
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 64
#define NUM_LEDS 357

CRGB leds_plus_safety_pixel[NUM_LEDS + 1];
CRGB *const leds(leds_plus_safety_pixel + 1);
const uint8_t kMatrixWidth = 17;
const uint8_t kMatrixHeight = 21;
const bool kMatrixSerpentineLayout = false;

uint16_t XY(uint8_t x, uint8_t y)
{
    uint16_t i;

    if (kMatrixSerpentineLayout == false)
    {
        i = (y * kMatrixWidth) + x;
    }

    if (kMatrixSerpentineLayout == true)
    {
        if (y & 0x01)
        {
            // Odd rows run backwards
            uint8_t reverseX = (kMatrixWidth - 1) - x;
            i = (y * kMatrixWidth) + reverseX;
        }
        else
        {
            // Even rows run forwards
            i = (y * kMatrixWidth) + x;
        }
    }

    return i;
}

uint16_t XYsafe(uint8_t x, uint8_t y)
{
    if (x >= kMatrixWidth)
        return -1;
    if (y >= kMatrixHeight)
        return -1;
    return XY(x, y);
}