#include "RFM69.h"

void configure_pins(void);

uint8_t is_pressed(uint8_t);

uint8_t cur_states = 0x00;
register_t portB = (register_t) 0x5005;
register_t portC = (register_t) 0x500A;
register_t portD = (register_t) 0x500F;

int main(void) {
  configure_pins();
  SPI_configure();
  RFM_configure(1);
  while (1) {
    uint8_t command = RFM_read_register(RFM_FIFO);
    *odr(portC) = (command << 2);
    if (command == 0) {
      invert_pin(odr(portD), 1);
      for (int volatile j=0; j<10000; ++ j);
    }
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
  }
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
  for (uint8_t i=3; i<7; ++ i) {
    if (i == 4) {
      continue;
    }
    set_one(ddr(portB), i);
    set_one(cr1(portB), i);
    set_one(cr2(portB), i);
  }
  set_one(odr(portB), 3);
  // PB7 as MISO
  set_zero(ddr(portB), 7);
  set_one(cr1(portB), 7);
  set_zero(cr2(portB), 7);

  RFM_set_NSS(portB, 3);
}
