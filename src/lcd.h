///////////////////////////////////////////
// ST7735 LCD Driver
//
// Public interface for the LCD.
///////////////////////////////////////////
#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

///////////////////////////////////////////
// LCD Control Pins
///////////////////////////////////////////
const uint8_t LCD_CS_PIN  = 6;
const uint8_t LCD_DC_PIN  = 5;
const uint8_t LCD_RST_PIN = 4;

///////////////////////////////////////////
// LCD Dimensions
///////////////////////////////////////////
constexpr uint16_t LCD_WIDTH  = 128;
constexpr uint16_t LCD_HEIGHT = 160;

constexpr uint8_t LCD_X_OFFSET = 2;
constexpr uint8_t LCD_Y_OFFSET = 1;

///////////////////////////////////////////
// ST7735 Commands
///////////////////////////////////////////
enum ST7735Command : uint8_t
{
    ST7735_SWRESET = 0x01,   // Software Reset
    ST7735_SLPOUT  = 0x11,   // Sleep Out

    ST7735_CASET   = 0x2A,   // Column Address Set
    ST7735_RASET   = 0x2B,   // Row Address Set
    ST7735_RAMWR   = 0x2C,   // Memory Write

    ST7735_MADCTL  = 0x36,   // Memory Access Control
    ST7735_COLMOD  = 0x3A,   // Interface Pixel Format

    ST7735_DISPON  = 0x29    // Display ON
};

///////////////////////////////////////////
// RGB565 Colors
///////////////////////////////////////////
constexpr uint16_t LCD_BLACK   = 0x0000;
constexpr uint16_t LCD_WHITE   = 0xFFFF;

constexpr uint16_t LCD_RED     = 0xF800;
constexpr uint16_t LCD_GREEN   = 0x07E0;
constexpr uint16_t LCD_BLUE    = 0x001F;

constexpr uint16_t LCD_YELLOW  = 0xFFE0;
constexpr uint16_t LCD_CYAN    = 0x07FF;
constexpr uint16_t LCD_MAGENTA = 0xF81F;

///////////////////////////////////////////
// Public Functions
///////////////////////////////////////////
void lcdInit();

void lcdDrawPixel(
    uint16_t x,
    uint16_t y,
    uint16_t color);

void lcdFillRect(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    uint16_t color);

void lcdFillScreen(uint16_t color);

void lcdDrawFastHLine(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t color);

void lcdDrawFastVLine(
    uint16_t x,
    uint16_t y,
    uint16_t height,
    uint16_t color);

///////////////////////////////////////////
// Private Functions
///////////////////////////////////////////
void lcdReset();

void lcdSelect();
void lcdDeselect();

void lcdCommandMode();
void lcdDataMode();

void lcdWriteCommand(uint8_t cmd);
void lcdWriteData(uint8_t data);
void lcdWriteColor(uint16_t color);
void lcdDelay(uint16_t ms);

// bool lcdIsValidCoordinate(uint16_t x, uint16_t y);
void lcdSetAddressWindow(
    uint16_t x0,
    uint16_t y0,
    uint16_t x1,
    uint16_t y1);


#endif