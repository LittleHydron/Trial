#include "RFM69.h"

void configure_pins(void);

register_t portA = (register_t) 0x5000;
register_t portB = (register_t) 0x5005;
register_t portC = (register_t) 0x500A;
register_t portD = (register_t) 0x500F;
register_t EXTI_CR1 = (register_t)0x50A0;
register_t EXTI_CR2 = (register_t)0x50A1;
register_t EXTI_SR1 = (register_t)0x50A3;
register_t CPU_CCR = (register_t)0x7F0A;

int main(void) {
  configure_pins();
  set_one(odr(portC), 6);
  set_one(odr(portC), 5);
  RFM_configure(0);
  
  while (1) {
   // __asm("WFI");
  }
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
  
  // PB3 is used as NSS
  set_one(ddr(portB), 3);
  set_one(cr1(portB), 3);
  set_one(cr2(portB), 3);
  set_one(odr(portB), 3);
  RFM_set_NSS(portB, 3);
  
  __asm("rim");
}


#pragma vector = 12
__interrupt void EXTI2_Handler(void) {
  RFM_write_register(RFM_FIFO, 2);
  set_one(odr(portD), 1);
  for (uint32_t volatile i=0; i<10000; ++ i);
  set_zero(odr(portD), 1);
  set_one(EXTI_SR1, 2);
}

#pragma vector = 13
__interrupt void EXTI3_Handler(void) {
  RFM_write_register(RFM_FIFO, 3);
  set_one(odr(portD), 1);
  for (uint32_t volatile i=0; i<10000; ++ i);
  set_zero(odr(portD), 1);
  set_one(EXTI_SR1, 3);
}

#pragma vector = 14
__interrupt void EXTI4_Handler(void) {
  RFM_write_register(RFM_FIFO, 4);
  set_one(odr(portD), 1);
  for (uint32_t volatile i=0; i<10000; ++ i);
  set_zero(odr(portD), 1);
  set_one(EXTI_SR1, 4);
}

#pragma vector = 15
__interrupt void EXTI5_Handler(void) {
  RFM_write_register(RFM_FIFO, 5);
  set_one(odr(portD), 1);
  for (uint32_t volatile i=0; i<10000; ++ i);
  set_zero(odr(portD), 1);
  set_one(EXTI_SR1, 5);
}
