#include <Arduino.h>

#include "benchmark.h"
#include "lcd.h"
#include "grahpics.h"

void benchmarkRun()
{

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

    for(uint8_t i=0; i < COLOR_COUNT; i++)
    {
        
        uint32_t start = micros();

        lcdFillScreen(LCD_COLORS[i]);

        uint32_t elapsed = micros() - start;
        
        Serial.printf("Fill Screen: %lu ms\n", elapsed);
    }
}