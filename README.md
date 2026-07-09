# ESP32 ST7735 Graphics Library

A lightweight graphics library for the **ESP32-S3** that implements an SPI driver, an ST7735 LCD driver, and a collection of 2D graphics algorithms written from scratch.

This project was developed as a learning exercise in embedded firmware development, with an emphasis on modular driver design, reusable APIs, and efficient integer graphics algorithms.

---

## Features

### SPI Driver

* Hardware SPI interface
* Configurable SPI clock
* Transaction support
* Single-byte and buffer transfers

### ST7735 LCD Driver

* Display initialization
* Address window management
* Pixel drawing
* Fast horizontal and vertical lines
* Rectangle drawing
* Screen fill
* RGB565 color support

### Graphics Library

* Bresenham line drawing
* Rectangle drawing
* Midpoint circle algorithm
* Filled circles using scanlines
* Triangle drawing
* Filled triangle rasterization using integer scanlines
* Monochrome bitmap rendering
* 5×7 bitmap font rendering
* ASCII string printing
* Multi-line text (`\n`) support

---

## Hardware

* ESP32-S3 Development Board
* ST7735 SPI TFT Display (128×160)

---

## Software

* PlatformIO
* Visual Studio Code
* Arduino Framework

---

## Future Improvements

* Text scaling
* Bitmap clipping
* Sprite rendering
* Display rotation
* Double buffering
* DMA-based SPI transfers
* Additional bitmap fonts
* ST7789 compatibility
* GUI widgets (buttons, labels, menus)

---

## License

This project is released under the MIT License.
