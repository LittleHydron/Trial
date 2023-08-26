#include "RegisterOps.h"

void invert_pin(register_t reg, uint8_t pin) {
  *reg ^= (1 << pin);
}

void set_one(register_t reg, uint8_t pin) {
  *reg |= (1 << pin);
}

void set_zero(register_t reg, uint8_t pin) {
  *reg &= ~(1 << pin);
}

register_t odr(register_t reg) {
  return reg;
}

iregister_t idr(register_t reg) {
  return reg + 0x01;
}

register_t ddr(register_t reg) {
  return reg + 0x02;
}

register_t cr1(register_t reg) {
  return reg + 0x03;
}

register_t cr2(register_t reg) {
  return reg + 0x04;
}