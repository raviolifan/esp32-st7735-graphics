#ifndef SPIMODULE_H
#define SPIMODULE_H

#include <Arduino.h>

void spiInit();
void spiWrite(uint8_t data);
void spiWriteBuffer(const uint8_t *buffer, size_t length);

///////////////////////////////////////////
// SPI Transaction Control
///////////////////////////////////////////
void spiBeginTransaction();
void spiEndTransaction();

#endif