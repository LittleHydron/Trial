#include "RFM69.h"

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
  select();
  uint8_t cur_mode = RFM_read_register(RFM_MODE);
  if (is_Rx) {
    cur_mode |= 0x04 << 2;
  } else {
    cur_mode |= 0x03 << 2;
  }
  RFM_write_register(RFM_MODE, cur_mode);
  
  // uint32_t frf = (217UL << 16); // 868 MHz
  // uint8_t frfMsb = (uint8_t)((frf >> 16) & 0xFF);
  // uint8_t frfMid = (uint8_t)((frf >> 8) & 0xFF);
  // uint8_t frfLsb = (uint8_t)(frf & 0xFF);
  // SPI_send_byte(FRF_MSB, frfMsb);
  // SPI_send_byte(FRF_MID, frfMid);
  // SPI_send_byte(FRF_LSB, frfLsb);
  
  cur_mode = RFM_read_register(PCT_CFG);
  cur_mode |= (1 << 7); // Variable packet size
  RFM_write_register(PCT_CFG, cur_mode);
  unselect();
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