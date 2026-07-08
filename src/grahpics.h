#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>

///////////////////////////////////////////
// Graphics Initialization
///////////////////////////////////////////
void graphicsInit();

///////////////////////////////////////////
// Line Primitives
///////////////////////////////////////////
void graphicsDrawLine(
    int x0,
    int y0,
    int x1,
    int y1,
    uint16_t color);

///////////////////////////////////////////
// Rectangle Primitives
///////////////////////////////////////////
void graphicsDrawRect(
    int x,
    int y,
    int width,
    int height,
    uint16_t color);

///////////////////////////////////////////
// Circle Primitives
///////////////////////////////////////////
void graphicsDrawCircle(
    int xc,
    int yc,
    int radius,
    uint16_t color);

void graphicsFillCircle(
    int xc,
    int yc,
    int radius,
    uint16_t color);

///////////////////////////////////////////
// Triangle Primitives
///////////////////////////////////////////
void graphicsDrawTriangle(
    int x0,
    int y0,
    int x1,
    int y1,
    int x2,
    int y2,
    uint16_t color);

///////////////////////////////////////////
// graphicsFillTriangle
///////////////////////////////////////////
void graphicsFillTriangle(
    int x0,
    int y0,
    int x1,
    int y1,
    int x2,
    int y2,
    uint16_t color);

///////////////////////////////////////////
// graphicsDrawBitmap
///////////////////////////////////////////
void graphicsDrawBitmap(
    int x,
    int y,
    int width,
    int height,
    const uint8_t *bitmap,
    uint16_t color);

///////////////////////////////////////////
// graphicsDrawChar
///////////////////////////////////////////
void graphicsDrawChar(
    int x,
    int y,
    char c,
    uint16_t color);

///////////////////////////////////////////
// graphicsPrint
///////////////////////////////////////////
void graphicsPrint(
    int x,
    int y,
    const char *text,
    uint16_t color);


const uint8_t smiley[] =
{
    0b00111100,
    0b01000010,
    0b10100101,
    0b10000001,
    0b10100101,
    0b10011001,
    0b01000010,
    0b00111100
};

#endif