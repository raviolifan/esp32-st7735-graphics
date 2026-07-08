///////////////////////////////////////////
// 
///////////////////////////////////////////

#include <Arduino.h>

#include "grahpics.h"
#include "spimodule.h"
#include "lcd.h"

void setup()
{
    Serial.begin(115200);   // Initialize serial communication

    spiInit();

    lcdInit();    

    lcdFillScreen(LCD_BLACK);

    graphicsInit();
}

const uint16_t LCD_COLORS[] =
{
    LCD_RED,
    LCD_GREEN,
    LCD_BLUE,
    LCD_YELLOW,
    LCD_CYAN,
    LCD_MAGENTA,
    LCD_WHITE,
    LCD_BLACK
};

constexpr uint8_t COLOR_COUNT = sizeof(LCD_COLORS) / sizeof(LCD_COLORS[0]);

void loop()
{
    for(uint8_t i = 0; i < COLOR_COUNT; i++)
    {
        // Fill screen
        lcdFillScreen(LCD_COLORS[i]); 
        
        // Draw star
        graphicsDrawLine(0, 0, 127, 159, LCD_COLORS[(i + 1) % COLOR_COUNT]);        // Top-left → Bottom-right
        graphicsDrawLine(127, 0, 0, 159, LCD_COLORS[(i + 2) % COLOR_COUNT]);        // Top-right → Bottom-left
        graphicsDrawLine(0, 80, 127, 80, LCD_COLORS[(i + 3) % COLOR_COUNT]);        // Horizontal
        graphicsDrawLine(64, 0, 64, 159, LCD_COLORS[(i + 4) % COLOR_COUNT]);        // Vertical
        
        // Draw rectangles
        graphicsDrawRect(10, 10, 50, 30, LCD_COLORS[(i + 5) % COLOR_COUNT]);
        graphicsDrawRect(30, 50, 20, 20, LCD_COLORS[(i + 6) % COLOR_COUNT]);
        graphicsDrawRect(70, 20, 40, 60, LCD_COLORS[(i + 7)% COLOR_COUNT]);

        // Draw circles
        graphicsFillCircle(64, 80, 5, LCD_COLORS[(i + 8) % COLOR_COUNT]);
        graphicsDrawCircle(64, 80, 10, LCD_COLORS[(i + 9) % COLOR_COUNT]);
        graphicsDrawCircle(64, 80, 20, LCD_COLORS[(i + 10) % COLOR_COUNT]);
        graphicsDrawCircle(64, 80, 30, LCD_COLORS[(i + 11) % COLOR_COUNT]);

        // Draw triangle
        graphicsDrawTriangle(64, 20, 20, 120, 108, 120, LCD_COLORS[(i + 12) % COLOR_COUNT]);
        
        // Load bitmap
        graphicsDrawBitmap( 20, 20, 8, 8, smiley, LCD_COLORS[(i + 13) % COLOR_COUNT]);

        // Print characters
        graphicsDrawChar( 40, 20, '%', LCD_RED);    
        graphicsDrawChar( 60, 20, 'C', LCD_GREEN);
        graphicsDrawChar( 80, 20, '1', LCD_BLUE);

        // Print string
        graphicsPrint(50, 70, "HELLO \nWORLD", LCD_WHITE);

        delay(1000);
    }
}

