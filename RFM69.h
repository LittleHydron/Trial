#ifndef RFM69_H

#define RFM69_H

#include "SPI.h"

#define RFM_FIFO (uint8_t) 0x00
#define RFM_MODE (uint8_t) 0x01
#define FRF_MSB  (uint8_t) 0x07
#define FRF_MID  (uint8_t) 0x08
#define FRF_LSB  (uint8_t) 0x09
#define PCT_CFG  (uint8_t) 0x37

void RFM_configure(void);
void RFM_write_register(uint8_t, uint8_t);
void select(void);
void unselect(void);
void RFM_set_NSS(register_t, uint8_t);
uint8_t RFM_read_register(uint8_t);

#endif