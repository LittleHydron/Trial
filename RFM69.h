#ifndef RFM69_H

#define RFM69_H

#include "SPI.h"

#define RFM_FIFO (uint8_t) 0x00
#define RFM_MODE (uint8_t) 0x01
#define FRF_MSB  (uint8_t) 0x07
#define FRF_MID  (uint8_t) 0x08
#define FRF_LSB  (uint8_t) 0x09
#define PCT_CFG  (uint8_t) 0x37

void RFM_configure(uint8_t);
void RFM_write_register(uint8_t, uint8_t);
void select(void);
void unselect(void);
void RFM_set_NSS(register_t, uint8_t);
uint8_t RFM_read_register(uint8_t);

register_t *NSS_port;
uint8_t NSS_pin;

void RFM_set_NSS(register_t port, uint8_t pin) {
  NSS_port = &port;
  NSS_pin = pin;
}

void select(void) {
  set_zero(odr(*NSS_port), NSS_pin);
}

void unselect(void) {
  set_one(odr(*NSS_port), NSS_pin);
}

void RFM_configure(uint8_t is_Rx) {
  uint8_t cur_mode = RFM_read_register(RFM_MODE);
  if (is_Rx) {
    cur_mode |= 0x04 << 2;
  } else {
    cur_mode |= 0x03 << 2;
  }
  RFM_write_register(RFM_MODE, cur_mode);
  
  cur_mode = RFM_read_register(PCT_CFG);
  cur_mode |= (1 << 7); // Variable packet size
  RFM_write_register(PCT_CFG, cur_mode);
}

uint8_t RFM_read_register(uint8_t addr) {
  select();
  SPI_send(addr & 0x7F);
  uint8_t data = SPI_read();
  unselect();
  return data;
}

void RFM_write_register(uint8_t addr, uint8_t val) {
  select();
  SPI_send(addr | 0x80);
  SPI_send(val);
  unselect();
}

#endif