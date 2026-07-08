///////////////////////////////////////////
// ST7735 LCD Driver
//
// Controls the LCD display.
///////////////////////////////////////////
#include <Arduino.h>

#include "lcd.h"
#include "spimodule.h"
#include "font5x7.h"

///////////////////////////////////////////
// Reset LCD
///////////////////////////////////////////
void lcdReset()
{
    digitalWrite(LCD_RST_PIN, LOW);
    lcdDelay(10);

    digitalWrite(LCD_RST_PIN, HIGH);
    lcdDelay(120);

    Serial.println("LCD Reset Complete");
}

///////////////////////////////////////////
// lcdDelay
//
// Delays for the specified number of milliseconds.
///////////////////////////////////////////
void lcdDelay(uint16_t ms)
{
    delay(ms);
}

///////////////////////////////////////////
// lcdCommandMode
//
// Turns DC pin Low
///////////////////////////////////////////
void lcdCommandMode()
{
    digitalWrite(LCD_DC_PIN, LOW);
}

///////////////////////////////////////////
// lcdDatadMode
//
// Turns DC pin High
///////////////////////////////////////////
void lcdDataMode()
{
    digitalWrite(LCD_DC_PIN, HIGH);
}

///////////////////////////////////////////
// lcdWriteCommand
//
// Sends a single command byte to the
// ST7735 controller.
///////////////////////////////////////////
void lcdWriteCommand(uint8_t command)
{
    lcdCommandMode();

    spiWrite(command);
}

///////////////////////////////////////////
// lcdWriteData 
//
// Sends a single data byte to the ST7735
// controller.
///////////////////////////////////////////
void lcdWriteData(uint8_t data)
{
    lcdDataMode();

    spiWrite(data);
}

///////////////////////////////////////////
// LCD Initialization
///////////////////////////////////////////
void lcdInit()
{
    pinMode(LCD_CS_PIN, OUTPUT);                // Configure GPIO
    pinMode(LCD_DC_PIN, OUTPUT);
    pinMode(LCD_RST_PIN, OUTPUT);

    digitalWrite(LCD_CS_PIN, HIGH);             // Set CS high

    lcdSelect();                                 // Select the LCD

    lcdReset();                                 // Reset the display

    lcdWriteCommand(ST7735_SWRESET);            // Software Reset
    lcdDelay(150);

    lcdWriteCommand(ST7735_SLPOUT);             // Exit Sleep
    lcdDelay(150);

    lcdWriteCommand(ST7735_COLMOD);             // Set Color Mode
    lcdWriteData(0x05);

    lcdDelay(10);

    lcdWriteCommand(ST7735_MADCTL);             // Memory Access Control
    lcdWriteData(0x00);

    lcdWriteCommand(ST7735_DISPON);             // Display ON
    
    lcdDelay(100);
    
    lcdDeselect();                                 // Deselect the LCD

    Serial.println("LCD Initialized");
}

/////////////////////////////////////////////
// lcdWriteUInt16
//
// Sends a 16-bit unsigned integer to the
// ST7735 controller as two separate bytes.
/////////////////////////////////////////////
static void lcdWriteUInt16(uint16_t value)
{
    lcdWriteData(value >> 8);
    lcdWriteData(value & 0xFF);
}

///////////////////////////////////////////////////////////////
// ////////////////////////////////////////////
// // lcdIsValidCoordinate
// //
// // Checks if the given (x, y) coordinates are
// // within the bounds of the LCD display.
// ///////////////////////////////////////////
// bool lcdIsValidCoordinate(uint16_t x, uint16_t y)
// {
//     return (x < LCD_WIDTH) && (y < LCD_HEIGHT);
// }
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////
// lcdWriteColor
//
// Sends a 16-bit color value to the ST7735
///////////////////////////////////////////
void lcdWriteColor(uint16_t color)
{
    lcdWriteData(color >> 8);                   // Send high byte
    lcdWriteData(color & 0xFF);                 // Send low byte
}


///////////////////////////////////////////
// Set Drawing Area
//
// Sets the area of the display where pixel
// data will be written. The area is defined
// by the top-left (x0, y0) and bottom-right
// (x1, y1) coordinates.
///////////////////////////////////////////
void lcdSetAddressWindow(
    uint16_t x0,
    uint16_t y0,
    uint16_t x1,
    uint16_t y1)
{
    lcdWriteCommand(ST7735_CASET);              // Column Address Set
    
    x0 += LCD_X_OFFSET;
    x1 += LCD_X_OFFSET;
    lcdWriteUInt16(x0);
    lcdWriteUInt16(x1);


    lcdWriteCommand(ST7735_RASET);              // Row Address Set
    
    y0 += LCD_Y_OFFSET;
    y1 += LCD_Y_OFFSET;
    lcdWriteUInt16(y0);
    lcdWriteUInt16(y1);


    lcdWriteCommand(ST7735_RAMWR);              // Memory Write
}

///////////////////////////////////////////
// Draw Pixel
// 
// Draws a single pixel at the specified
// (x, y) coordinates with the given color. 
///////////////////////////////////////////
void lcdDrawPixel(
    uint16_t x,
    uint16_t y,
    uint16_t color)
{    
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)      // Check bounds
    {
        return;
    }

    lcdSelect();

    lcdSetAddressWindow(x, y, x, y);

    lcdWriteColor(color);

    lcdDeselect();
}

///////////////////////////////////////////
// Select LCD
//
// Selects the LCD by pulling the CS pin low
// and begins an SPI transaction.
///////////////////////////////////////////
void lcdSelect()
{
    spiBeginTransaction();

    digitalWrite(LCD_CS_PIN, LOW);
}

///////////////////////////////////////////
// Deselect LCD
//
// Deselects the LCD by pulling the CS pin high
// and ends the SPI transaction.
///////////////////////////////////////////
void lcdDeselect()
{
    digitalWrite(LCD_CS_PIN, HIGH);
        
    spiEndTransaction();
}

////////////////////////////////////////////
// lcdFillRect
//
// Fills a rectangle starting at (x, y) with the
// specified width, height, and color.
///////////////////////////////////////////
void lcdFillRect(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    uint16_t color)
{
    if (width == 0 || height == 0)              // If width or height is zero, no need to draw
        return;    
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT)      // If x or y is out of bounds, no need to draw
        return;

    if (x + width > LCD_WIDTH)                  // Adjust width if it exceeds the display boundary
        width = LCD_WIDTH - x;
    if (y + height > LCD_HEIGHT)                // Adjust height if it exceeds the display boundary
        height = LCD_HEIGHT - y;

    lcdSelect();

    lcdSetAddressWindow(x, y, 
        x + width - 1, y + height - 1);

    for (uint16_t j = 0; j < height; j++)       // Iterate through each row
    {
        for (uint16_t i = 0; i < width; i++)    // Iterate through each column
        {
            lcdWriteColor(color);
        }
    }

    lcdDeselect();
}

///////////////////////////////////////////
// Fast Horizontal Line
///////////////////////////////////////////
void lcdDrawFastHLine(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t color)
{
        lcdFillRect(
        x,
        y,
        width,
        1,
        color);
}

///////////////////////////////////////////
// Fast Vertical Line
///////////////////////////////////////////
void lcdDrawFastVLine(
    uint16_t x,
    uint16_t y,
    uint16_t height,
    uint16_t color)
{
    lcdFillRect(
        x,
        y,
        1,
        height,
        color);
}

////////////////////////////////////////////
// lcdFillScreen
//
// Fills the entire screen with a single color.
///////////////////////////////////////////
void lcdFillScreen(uint16_t color)
{
    lcdFillRect(
        0,
        0,
        LCD_WIDTH,
        LCD_HEIGHT,
        color);
}

