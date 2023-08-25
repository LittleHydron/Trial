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

#endif