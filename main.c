#include <stdint.h>

void delay(void);
void configure_pins(void);
void set_one(uint8_t *, uint8_t);
void set_zero(uint8_t *, uint8_t);

uint8_t *odr(uint8_t *);
uint8_t volatile *idr(uint8_t *);
uint8_t *ddr(uint8_t *);
uint8_t *cr1(uint8_t *);
uint8_t *cr2(uint8_t *);
uint8_t is_pressed(uint8_t);
uint8_t prev_state(uint8_t);

uint8_t prev_states = 0x00;
uint8_t * const portA = (uint8_t *) 0x5000;
uint8_t * const portC = (uint8_t *) 0x500A;
uint8_t * const portD = (uint8_t *) 0x500F;
uint8_t * const EXTI_CR1 = (uint8_t *)0x50A0;
uint8_t * const EXTI_CR2 = (uint8_t *)0x50A1;
uint8_t * const EXTI_SR1 = (uint8_t *)0x50A3;
uint8_t * const CPU_CCR = (uint8_t *)0x7F0A;

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

void set_one(uint8_t * reg, uint8_t pin) {
  *reg |= (1 << pin);
}

void set_zero(uint8_t * reg, uint8_t pin) {
  *reg &= ~(1 << pin);
}

uint8_t *odr(uint8_t * reg) {
  return reg;
}

uint8_t volatile *idr(uint8_t * reg) {
  return reg + 0x01;
}

uint8_t *ddr(uint8_t * reg) {
  return reg + 0x02;
}

uint8_t *cr1(uint8_t * reg) {
  return reg + 0x03;
}

uint8_t *cr2(uint8_t * reg) {
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
