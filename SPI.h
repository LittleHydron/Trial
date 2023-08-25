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

#endif