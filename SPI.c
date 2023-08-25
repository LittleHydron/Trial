#include "SPI.h"

void SPI_configure(void) {
  *SPI_CR1 &= ~(7 << 3);    // f/2 baud rate mode
  *SPI_CR1 &= ~3;           // CPOL and CPHA conf
  set_zero(SPI_CR1, 7);     // MSB First (RFM requires that)
  *SPI_CR1 |= 3;            // software slave select & master mode
  set_one(SPI_CR1, 1 << 6); // SPI enable
  set_one(SPI_CR1, 1 << 2); // Master mode
}

void SPI_send(uint8_t data) {
  while(!(*SPI_SR & (1 << 1)));
  *SPI_DR = (data);
}

uint8_t SPI_read(void) {
  return *SPI_DR;
}