#ifndef REGISTEROPS_H

#define REGISTEROPS_H

#include <stdint.h>

typedef uint8_t*const register_t;
typedef uint8_t volatile* iregister_t;

void set_one(register_t, uint8_t);
void set_zero(register_t, uint8_t);
void invert_pin(register_t, uint8_t);

register_t odr(register_t);
iregister_t idr(register_t);
register_t ddr(register_t);
register_t cr1(register_t);
register_t cr2(register_t);

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

#endif