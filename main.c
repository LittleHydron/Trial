#include <stdint.h>

typedef uint8_t*const register_t;
typedef uint8_t volatile* iregister_t;

void delay(void);
void configure_pins(void);
void set_one(register_t, uint8_t);
void set_zero(register_t, uint8_t);

register_t odr(register_t);
iregister_t idr(register_t);
register_t ddr(register_t);
register_t cr1(register_t);
register_t cr2(register_t);
uint8_t is_pressed(uint8_t);
uint8_t prev_state(uint8_t);

uint8_t prev_states = 0x00;
register_t portA = (register_t) 0x5000;
register_t portC = (register_t) 0x500A;
register_t portD = (register_t) 0x500F;
register_t EXTI_CR1 = (register_t)0x50A0;
register_t EXTI_CR2 = (register_t)0x50A1;
register_t EXTI_SR1 = (register_t)0x50A3;
register_t CPU_CCR = (register_t)0x7F0A;

int main(void) {
  configure_pins();
  while (1) {
    __asm("WFI");
  }
}

uint8_t prev_state(uint8_t button) {
  return prev_states & (1 << button);
}

uint8_t is_pressed(uint8_t button) {
  return !(*idr(portA) & (1 << button));
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

void configure_pins(void) {
  set_one(ddr(portD), 1);
  set_one(cr1(portD), 1);
  set_one(cr2(portD), 1);
  for (uint8_t i=2; i<7; ++ i) {
    set_one(ddr(portC), i);
    set_one(cr1(portC), i);
    set_one(cr2(portC), i);
  }
  
  for (uint8_t i=2; i<6; ++ i) {
    set_zero(ddr(portA), i);
    set_one(cr1(portA), i);
    set_one(cr2(portA), i);
  }
  
  *CPU_CCR |= (1 << 3) | (1 << 5);
  
  *EXTI_CR1 |= 0xF0;
  *EXTI_CR2 |= 0x0F;

  __asm("rim");
}

#pragma vector = 12
__interrupt void EXTI2_Handler(void) {
  if (is_pressed(2)) {
    set_one(&prev_states, 2);
    
    set_one(odr(portC), 2);
    set_one(odr(portC), 3);
    set_one(odr(portC), 4);
  } else {
    set_zero(&prev_states, 2);
    
    if (!prev_state(4)) {
      set_zero(odr(portC), 2);
    }
    if (!prev_state(3)) {
      set_zero(odr(portC), 3);
    }
    if (!prev_state(4)) {
      set_zero(odr(portC), 4);
    }
  }
  set_one(EXTI_SR1, 2);
}

#pragma vector = 13
__interrupt void EXTI3_Handler(void) {
  if (is_pressed(3)) {
    set_one(&prev_states, 3);
    
    set_one(odr(portC), 3);
    set_one(odr(portC), 5);
    set_one(odr(portD), 1);
  } else {
    set_zero(&prev_states, 3);
    
    if (!prev_state(2)) {
      set_zero(odr(portC), 3);
    }
    if (!prev_state(5)) {
      set_zero(odr(portC), 5);
    }
    if (!prev_state(5)) {
      set_zero(odr(portD), 1);
    }
  }
  set_one(EXTI_SR1, 3);
}

#pragma vector = 14
__interrupt void EXTI4_Handler(void) {
  if (is_pressed(4)) {
    set_one(&prev_states, 4);
    
    set_one(odr(portC), 2);
    set_one(odr(portC), 4);
    set_one(odr(portC), 6);
  } else {
    set_zero(&prev_states, 4);
    
    if (!prev_state(2)) {
      set_zero(odr(portC), 2);
    }
    if (!prev_state(2)) {
      set_zero(odr(portC), 4);
    }
    if (!prev_state(5)) {
      set_zero(odr(portC), 6);
    }
  }
  set_one(EXTI_SR1, 4);
}

#pragma vector = 15
__interrupt void EXTI5_Handler(void) {
  if (is_pressed(5)) {
    set_one(&prev_states, 5);
    
    set_one(odr(portC), 5);
    set_one(odr(portC), 6);
    set_one(odr(portD), 1);
  } else {
    set_zero(&prev_states, 5);
    
    if (!prev_state(3)) {
      set_zero(odr(portC), 5);
    }
    if (!prev_state(4)) {
      set_zero(odr(portC), 6);
    }
    if (!prev_state(3)) {
      set_zero(odr(portD), 1);
    }
  }
  set_one(EXTI_SR1, 5);
}
