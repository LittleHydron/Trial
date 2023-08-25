#include "RFM69.h"

void delay(void);
void configure_pins(void);

uint8_t is_pressed(uint8_t);

uint8_t cur_states = 0x00;
register_t portB = (register_t) 0x5005;
register_t portC = (register_t) 0x500A;
register_t portD = (register_t) 0x500F;

int main(void) {
  configure_pins();
  RFM_configure();
  while (1) {
    uint8_t command = RFM_read_register(RFM_FIFO);
    invert_pin(&cur_states, command);
    switch(command) {
    case 2:
      if (is_pressed(2)) {
        set_one(odr(portC), 2);
        set_one(odr(portC), 3);
        set_one(odr(portC), 4);
      } else {
        if (!is_pressed(4)) {
          set_zero(odr(portC), 2);
        }
        if (!is_pressed(3)) {
          set_zero(odr(portC), 3);
        }
        if (!is_pressed(4)) {
          set_zero(odr(portC), 4);
        }
      }
      break;
    case 3:
      if (is_pressed(3)) {
        set_one(odr(portC), 3);
        set_one(odr(portC), 5);
        set_one(odr(portD), 1);
      } else {
        if (!is_pressed(2)) {
          set_zero(odr(portC), 3);
        }
        if (!is_pressed(5)) {
          set_zero(odr(portC), 5);
        }
        if (!is_pressed(5)) {
          set_zero(odr(portD), 1);
        }
      }
      break;
    case 4:
      if (is_pressed(4)) {
        set_one(odr(portC), 2);
        set_one(odr(portC), 4);
        set_one(odr(portC), 6);
      } else {
        if (!is_pressed(2)) {
          set_zero(odr(portC), 2);
        }
        if (!is_pressed(2)) {
          set_zero(odr(portC), 4);
        }
        if (!is_pressed(5)) {
          set_zero(odr(portC), 6);
        }
      }
      break;
    case 5:
      if (is_pressed(5)) {
        set_one(odr(portC), 5);
        set_one(odr(portC), 6);
        set_one(odr(portD), 1);
      } else {
        if (!is_pressed(3)) {
          set_zero(odr(portC), 5);
        }
        if (!is_pressed(4)) {
          set_zero(odr(portC), 6);
        }
        if (!is_pressed(3)) {
          set_zero(odr(portD), 1);
        }
      }
      break;
    }
    delay();
  }
}

void delay(void) {
  for (volatile uint32_t i=0; i<100000; ++ i);
}

uint8_t is_pressed(uint8_t button) {
  return cur_states & (1 << button);
}

void configure_pins(void) {
  set_one(ddr(portD), 1);
  set_one(cr1(portD), 1);
  set_one(cr2(portD), 1);
  for (uint8_t i=2; i<7; ++ i) {
    set_one(ddr(portC), i);
    set_one(cr1(portC), i);
    set_one(cr2(portC), i);
  }
  
  // PB3 is used as NSS
  set_one(ddr(portB), 3);
  set_one(cr1(portB), 3);
  set_one(cr2(portB), 3);
  set_one(odr(portB), 3);

  RFM_set_NSS(portB, 3);
}
