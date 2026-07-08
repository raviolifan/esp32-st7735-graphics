///////////////////////////////////////////
// FONT5X7.H
//
// Public interface for the 5x7 font.
///////////////////////////////////////////
#ifndef FONT5X7_H
#define FONT5X7_H

#pragma once

#include <stdint.h>

////////////////////////////////////////////
// Font Dimensions
///////////////////////////////////////////
constexpr uint8_t FONT_WIDTH  = 5;
constexpr uint8_t FONT_HEIGHT = 7;
constexpr uint8_t FONT_SPACING = 1;

constexpr uint8_t ASCII_FIRST = 32;
constexpr uint8_t ASCII_LAST = 126;
constexpr uint8_t ASCII_COUNT = ASCII_LAST - ASCII_FIRST + 1;

extern const uint8_t font5x7[95][7];

#endif // FONT5X7_H
