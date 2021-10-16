#include <matrix_setup.h>
#include <ezTime.h>

#pragma once

void displayDigit(int number, int x, int y, CRGB color) {
  uint16_t one[] = {XY(x + 2, y), XY(x + 2, y + 1), XY(x + 2, y + 2), XY(x + 2, y + 3), XY(x + 2, y + 4)};
  uint16_t two[] = {XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x, y + 1), XY(x + 2, y + 2), XY(x + 1, y + 2), XY(x, y + 2), XY(x + 2, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4)};
  uint16_t three[] = {XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x + 2, y + 1), XY(x + 2, y + 2), XY(x + 1, y + 2), XY(x, y + 2), XY(x + 2, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4)};
  uint16_t four[] = {XY(x + 2, y), XY(x + 2, y + 1), XY(x + 2, y + 2), XY(x + 1, y + 2), XY(x, y + 2), XY(x + 2, y + 3), XY(x, y + 3), XY(x, y + 4), XY(x + 2, y + 4)};
  uint16_t five[] = {XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x + 2, y + 1), XY(x + 2, y + 2), XY(x + 1, y + 2), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4)};
  uint16_t six[] = {XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x + 2, y + 1), XY(x, y + 1), XY(x + 2, y + 2), XY(x + 1, y + 2), XY(x, y + 2), XY(x, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4)};
  uint16_t seven[] = {XY(x + 2, y), XY(x + 2, y + 1), XY(x + 2, y + 2), XY(x + 2, y + 3), XY(x + 2, y + 4), XY(x + 1, y + 4), XY(x, y + 4)};
  uint16_t eight[] = {XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x + 2, y + 1), XY(x, y + 1), XY(x + 2, y + 2), XY(x + 1, y + 2), XY(x, y + 2), XY(x, y + 3), XY(x + 2, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4)};
  uint16_t nine[] = {XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x + 2, y + 1), XY(x + 2, y + 2), XY(x + 1, y + 2), XY(x, y + 2), XY(x, y + 3), XY(x + 2, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4)};
  uint16_t zero[] = {XY(x, y), XY(x + 1, y), XY(x + 2, y), XY(x + 2, y + 1), XY(x, y + 1), XY(x + 2, y + 2), XY(x, y + 2), XY(x, y + 3), XY(x + 2, y + 3), XY(x, y + 4), XY(x + 1, y + 4), XY(x + 2, y + 4)};


  if (number == 1) {
    for (int i = 0; i < (sizeof(one) / sizeof(one[0])); i++) {
      leds[one[i]] = color;
    }
  }
  if (number == 2) {
    for (int i = 0; i < (sizeof(two) / sizeof(two[0])); i++) {
      leds[two[i]] = color;
    }
  }
  if (number == 3) {
    for (int i = 0; i < (sizeof(three) / sizeof(three[0])); i++) {
      leds[three[i]] = color;
    }
  }
  if (number == 4) {
    for (int i = 0; i < (sizeof(four) / sizeof(four[0])); i++) {
      leds[four[i]] = color;
    }
  }
  if (number == 5) {
    for (int i = 0; i < (sizeof(five) / sizeof(five[0])); i++) {
      leds[five[i]] = color;
    }
  }
  if (number == 6) {
    for (int i = 0; i < (sizeof(six) / sizeof(six[0])); i++) {
      leds[six[i]] = color;
    }
  }
  if (number == 7) {
    for (int i = 0; i < (sizeof(seven) / sizeof(seven[0])); i++) {
      leds[seven[i]] = color;
    }
  }
  if (number == 8) {
    for (int i = 0; i < (sizeof(eight) / sizeof(eight[0])); i++) {
      leds[eight[i]] = color;
    }
  }
  if (number == 9) {
    for (int i = 0; i < (sizeof(nine) / sizeof(nine[0])); i++) {
      leds[nine[i]] = color;
    }
  }
  if (number == 0) {
    for (int i = 0; i < (sizeof(zero) / sizeof(zero[0])); i++) {
      leds[zero[i]] = color;
    }
  }
}

void displayClock(Timezone myTZ, CRGB color) {
  //display hours
  int h_tens, h_units;

  h_tens = (myTZ.hour() % 100) / 10;
  h_units = (myTZ.hour() % 10);

  displayDigit(h_tens, 5, 14, color);
  displayDigit(h_units, 9, 14, color);

  //display minutes
  int m_tens, m_units;

  m_tens = (myTZ.minute() % 100) / 10;
  m_units = (myTZ.minute() % 10);

  displayDigit(m_tens, 5, 8, color);
  displayDigit(m_units, 9, 8, color);

  //display seconds
  int s_tens, s_units;

  s_tens = (myTZ.second() % 100) / 10;
  s_units = (myTZ.second() % 10);

  displayDigit(s_tens, 5, 2, color);
  displayDigit(s_units, 9, 2, color);

  //Serial.println(String(h_tens) + String(h_units) + ":" + String(m_tens) + String(m_units) + ":" + String(s_tens) + String(s_units));
}