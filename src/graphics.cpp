///////////////////////////////////////////
// Graphics Library
//
// Implements graphics drawing algorithms
// using the LCD driver as the rendering
// backend.
///////////////////////////////////////////
#include <Arduino.h>

#include "grahpics.h"
#include "lcd.h"
#include "font5x7.h"

///////////////////////////////////////////
// Private Types
//
// Internal data structures used by the
// graphics algorithms.
///////////////////////////////////////////
struct Point
{
    int x;
    int y;
};

enum class GraphicsDrawMode
{
    Outline,
    Filled
};

///////////////////////////////////////////
// Private Helper Functions
///////////////////////////////////////////

///////////////////////////////////////////
// Swap Points
//
// Swaps the contents of two Point objects.
///////////////////////////////////////////
static void graphicsSwapPoint(
    Point &a,
    Point &b)
{
    Point temp = a;
    a = b;
    b = temp;
}

///////////////////////////////////////////
// Swap Integers
//
// Swaps the values of two integers.
///////////////////////////////////////////
static void graphicsSwap(
    int &a,
    int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

///////////////////////////////////////////
// Interpolate X
//
// Returns the x-coordinate on the line
// segment between two points for the
// specified y-coordinate.
///////////////////////////////////////////
static int graphicsEdgeInterpolateX(
    const Point &p0,
    const Point &p1,
    int y)
{
    if (p0.y == p1.y)
    {
        return p0.x;
    }

    return p0.x +
       ((y - p0.y) * (p1.x - p0.x))
       / (p1.y - p0.y);
}

///////////////////////////////////////////
// Public Functions
///////////////////////////////////////////

///////////////////////////////////////////
// Graphics Initialization
//
// Initializes the graphics library.
///////////////////////////////////////////
void graphicsInit()
{

}

///////////////////////////////////////////
// Draw Line
//
// Draws a line between two points using
// Bresenham's Line Algorithm.
///////////////////////////////////////////
void graphicsDrawLine(
    int x0,
    int y0,
    int x1,
    int y1,
    uint16_t color)
{
    int dx = abs(x1 - x0);              // Horizontal distance
    int dy = abs(y1 - y0);              // Vertical distance

    int sx = (x0 < x1) ? 1 : -1;        // +1 = right, -1 = left
    int sy = (y0 < y1) ? 1 : -1;        // +1 = down,  -1 = up

    int err = dx - dy;                  // Bresenham error term

    while (true)
    {
        lcdDrawPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1)       // Stop when the end point is reached
        {
            break;
        }

        int e2 = 2 * err;               // Double the error to avoid floating point

        if (e2 > -dy)                   // Step in the x-direction
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)                    // Step in the y-direction
        {
            err += dx;
            y0 += sy;
        }
    }
}

///////////////////////////////////////////
// Draw Rectangle
//
// Draws an outlined rectangle using four
// optimized horizontal and vertical lines.
///////////////////////////////////////////
void graphicsDrawRect(
    int x,
    int y,
    int width,
    int height,
    uint16_t color)
{
    if (width <= 0 || height <= 0)      // Ignore invalid dimensions
    {
        return;
    }

    lcdDrawFastHLine(                   // Top row
    x,
    y,
    width,
    color);

    lcdDrawFastHLine(                   // Bottom row
    x,
    y + height - 1,
    width,
    color);

    lcdDrawFastVLine(                   // Left edge
    x,
    y,
    height,
    color);

    lcdDrawFastVLine(                   // Right edge
    x + width - 1,
    y,
    height,
    color);
}

///////////////////////////////////////////
// Plot Circle Octants
//
// Plots the eight symmetric points of a
// circle for the given (x, y) coordinate.
///////////////////////////////////////////
static void graphicsPlotCircleOctants(
    int xc,
    int yc,
    int x,
    int y,
    uint16_t color)
{
    lcdDrawPixel(xc + x, yc + y, color);

    lcdDrawPixel(xc - x, yc + y, color);

    lcdDrawPixel(xc + x, yc - y, color);

    lcdDrawPixel(xc - x, yc - y, color);

    lcdDrawPixel(xc + y, yc + x, color);

    lcdDrawPixel(xc - y, yc + x, color);

    lcdDrawPixel(xc + y, yc - x, color);

    lcdDrawPixel(xc - y, yc - x, color);
}

static void graphicsFillCircleOctants(
    int xc,
    int yc,
    int x,
    int y,
    uint16_t color)
{
    lcdDrawFastHLine(
        xc - x,
        yc + y,
        (2 * x) + 1,
        color);
    lcdDrawFastHLine(
        xc - x,
        yc - y,
        (2 * x) + 1,
        color);
    lcdDrawFastHLine(
        xc - y,
        yc + x,
        (2 * y) + 1,
        color);
    lcdDrawFastHLine(
        xc - y,
        yc - x,
        (2 * y) + 1,
        color);
}


///////////////////////////////////////////
// Draw Circle
//
// Draws a circle centered at (xc, yc)
// using the Midpoint Circle Algorithm.
///////////////////////////////////////////
static void graphicsCircleInternal(
    int xc,
    int yc,
    int radius,
    uint16_t color,
    GraphicsDrawMode mode)
{
    if (radius < 0)
    {
        return;
    }

    int x = 0;                          // Start at the top of the circle
    int y = radius;                     // Current y position
    int d = 1 - radius;                 // Midpoint decision variable

    while (x <= y)                       // Calculate one octant and mirror it into the other seven
    {   
        if (d < 0)                      // Midpoint is inside of the circle
        {
            d += (2 * x) + 1;           // Move horizontally
        }
        else                            // Midpoint is outside of the circle
        {
            y--;                        // Move diagonally

            d += 2 * (x - y) + 1;       // Update decision variable
        }
        
        if (mode == GraphicsDrawMode::Filled)
        {
            graphicsFillCircleOctants(      
                xc,
                yc,
                x,
                y,
                color);        }
        else
        {
            graphicsPlotCircleOctants(      
                xc,
                yc,
                x,
                y,
                color);        }


        x++;                            // Advance to the next x position
    }
}

///////////////////////////////////////////
// Draw Circle
//
// Draws the outline of a circle centered
// at (xc, yc) using the Midpoint Circle
// Algorithm.
///////////////////////////////////////////
void graphicsDrawCircle(
    int xc,
    int yc,
    int radius,
    uint16_t color)
{
    graphicsCircleInternal(
        xc,
        yc,
        radius,
        color,
        GraphicsDrawMode::Outline);
}

///////////////////////////////////////////
// Fill Circle
//
// Draws a filled circle centered at
// (xc, yc) using the Midpoint Circle
// Algorithm and horizontal scanlines.
///////////////////////////////////////////
void graphicsFillCircle(
    int xc,
    int yc,
    int radius,
    uint16_t color)
{
    graphicsCircleInternal(
        xc,
        yc,
        radius,
        color,
        GraphicsDrawMode::Filled);
}

///////////////////////////////////////////
// Draw Triangle
//
// Draws the outline of a triangle by
// connecting its three vertices.
///////////////////////////////////////////
void graphicsDrawTriangle(
    int x0,
    int y0,
    int x1,
    int y1,
    int x2,
    int y2,
    uint16_t color)
{
    graphicsDrawLine(           // Draw first edge
    x0,
    y0,
    x1,
    y1,
    color);

    graphicsDrawLine(           // Draw second edge
    x1,
    y1,
    x2,
    y2,
    color);

    graphicsDrawLine(           // Draw third edge
    x2,
    y2,
    x0,
    y0,
    color);
}

///////////////////////////////////////////
// Fill Triangle
//
// Draws a filled triangle using integer
// scanline rasterization.
///////////////////////////////////////////
void graphicsFillTriangle(
    int x0,
    int y0,
    int x1,
    int y1,
    int x2,
    int y2,
    uint16_t color)
{
    Point p0 = { x0, y0 };                      // Triangle vertices
    Point p1 = { x1, y1 };
    Point p2 = { x2, y2 };

    ///////////////////////////////////////
    // Sort vertices by ascending y-value
    //
    // After sorting:
    // p0 = top
    // p1 = middle
    // p2 = bottom
    ///////////////////////////////////////
    if (p0.y > p1.y)
    {
        graphicsSwapPoint(p0, p1);
    }

    if (p1.y > p2.y)
    {
        graphicsSwapPoint(p1, p2);
    }

    if (p0.y > p1.y)
    {
        graphicsSwapPoint(p0, p1);
    }

    ///////////////////////////////////////
    // Fill Upper Half
    //
    // Rasterize scanlines from the top
    // vertex down to the middle vertex.
    ///////////////////////////////////////
    for (int y = p0.y; y <= p1.y; y++)
    {
        int leftX = graphicsEdgeInterpolateX(       // Left edge
            p0,
            p1,
            y);

        int rightX = graphicsEdgeInterpolateX(      // Right edge
            p0,
            p2,
            y);

        if (leftX > rightX)                     // Ensure left edge is first
        {
            graphicsSwap(leftX, rightX);
        }

        lcdDrawFastHLine(                       // Draw one horizontal scanline
            leftX,
            y,
            rightX - leftX + 1,
            color);
    }

    ///////////////////////////////////////
    // Fill Lower Half
    //
    // Rasterize scanlines from the middle
    // vertex down to the bottom vertex.
    ///////////////////////////////////////
    for (int y = p1.y; y <= p2.y; y++)
    {
        int leftX = graphicsEdgeInterpolateX(       // Left edge
            p1,
            p2,
            y);

        int rightX = graphicsEdgeInterpolateX(      // Right edge
            p0,
            p2,
            y);

        if (leftX > rightX)                     // Ensure left edge is first
        {
            graphicsSwap(leftX, rightX);
        }

        lcdDrawFastHLine(                       // Draw one horizontal scanline
            leftX,
            y,
            rightX - leftX + 1,
            color);
    }
}

///////////////////////////////////////////
// Draw Bitmap
//
// Draws a monochrome bitmap at the
// specified location. Set bits are drawn
// using the specified color, while
// cleared bits are left unchanged.
///////////////////////////////////////////
void graphicsDrawBitmap(
    int x,
    int y,
    int width,
    int height,
    const uint8_t *bitmap,
    uint16_t color)
{
    for (int row = 0; row < height; row++)          // Process each bitmap
    {
        uint8_t data = bitmap[row];                 // Read one row of bitmap data

        for (int bit = 0; bit < width; bit++)       // Process each bit in the row
        {
            if (data & (0x80 >> bit))               // Is the current pixel set?
            {
                lcdDrawPixel(
                    x + bit,
                    y + row,
                    color);
            }
        }
    }
}

///////////////////////////////////////////
// Draw Character
//
// Draws a 5x7 ASCII character using the
// built-in bitmap font.
///////////////////////////////////////////
void graphicsDrawChar(
    int x,
    int y,
    char c,
    uint16_t color)
{
    if (c < ASCII_FIRST || c > ASCII_LAST)              // Character is outside the font table
    {
        return;
    }

    int index = c - ASCII_FIRST;                        // Convert ASCII to font index
    const uint8_t *bitmap = font5x7[index];    

    for (int row = 0; row < FONT_HEIGHT; row++)         // Process each row of the character
    {
        uint8_t data = bitmap[row];

        for (int col = 0; col < FONT_WIDTH; col++)      // Process each column
        {
            if (data & (1 << (4 - col)))                // Is the current pixel set?
            {
                lcdDrawPixel(
                    x + col,
                    y + row,
                    color);
            }
        }
    }
}

///////////////////////////////////////////
// Print Text
//
// Draws a null-terminated ASCII string
// starting at the specified location.
// Supports newline characters ('\n') for
// multi-line text.
///////////////////////////////////////////
void graphicsPrint(
    int x,
    int y,
    const char *text,
    uint16_t color)
{
    int startX = x;                                 // Remember initial x position

    while (*text)                                   // Process each character
    {
        if (*text == '\n')                          // Move to the next line
        {
            x = startX;
            y += FONT_HEIGHT + FONT_SPACING;

            text++;

            continue;
        }
        graphicsDrawChar(                           // Draw current character
            x,
            y,
            *text,
            color);

        x += FONT_WIDTH + FONT_SPACING;             // Advance to next character position

        text++;                                     // Advance to next character
    }
}


