#ifndef SPI_H
#define SPI_H

#include "RegisterOps.h"

#define SPI_CR1 (register_t) 0x5200
#define SPI_CR2 (register_t) 0x5201
#define SPI_DR  (register_t) 0x5204
#define SPI_SR  (register_t) 0x5203

void SPI_configure(void);
void SPI_send(uint8_t);
uint8_t SPI_read(void);

void SPI_configure(void) {
  *SPI_CR1 &= ~(7 << 3);    // f/2 baud rate mode
  *SPI_CR1 &= ~3;           // CPOL and CPHA conf
  set_zero(SPI_CR1, 7);     // MSB First (RFM requires that)
  *SPI_CR1 |= 3;            // software slave select & master mode
  set_one(SPI_CR1, 1 << 6); // SPI enable
  set_one(SPI_CR1, 1 << 2); // Master mode
}

void SPI_send(uint8_t data) {
  // set_one(odr((register_t) 0x500A), 3);
  // for (uint32_t volatile i=0; i<10000; ++ i);
  // set_zero(odr((register_t) 0x500A), 3);
  while(!(*SPI_SR & (1 << 1)));
  // set_one(odr((register_t) 0x500A), 4);
  // for (uint32_t volatile i=0; i<10000; ++ i);
  // set_zero(odr((register_t) 0x500A), 4);
  *SPI_DR = (data);
}

uint8_t SPI_read(void) {
  return *SPI_DR;
}

#endif