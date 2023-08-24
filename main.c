#include <stdint.h>

typedef uint8_t*const register_t;
typedef uint8_t volatile* iregister_t;

void delay(void);
void configure_pins(void);
void configure_rfm(void);
void set_one(register_t, uint8_t);
void set_zero(register_t, uint8_t);
void SPI_send_byte(uint8_t, uint8_t);

register_t odr(register_t);
iregister_t idr(register_t);
register_t ddr(register_t);
register_t cr1(register_t);
register_t cr2(register_t);
uint8_t is_pressed(uint8_t);
uint8_t prev_state(uint8_t);
uint8_t SPI_read_byte(uint8_t);

uint8_t prev_states = 0x00;
register_t portA = (register_t) 0x5000;
register_t portB = (register_t) 0x5005;
register_t portC = (register_t) 0x500A;
register_t portD = (register_t) 0x500F;
register_t EXTI_CR1 = (register_t)0x50A0;
register_t EXTI_CR2 = (register_t)0x50A1;
register_t EXTI_SR1 = (register_t)0x50A3;
register_t CPU_CCR = (register_t)0x7F0A;

register_t SPI_CR1 = (register_t) 0x5200;
register_t SPI_CR2 = (register_t) 0x5201;
iregister_t SPI_DR = (register_t) 0x5204;
iregister_t SPI_SR = (register_t) 0x5203;

uint8_t const RFM_FIFO = 0x00;
uint8_t const RFM_MODE = 0x01;
uint8_t const FRF_MSB = 0x07;
uint8_t const FRF_MID = 0x08;
uint8_t const FRF_LSB = 0x09;
uint8_t const PCT_CFG = 0x37;

int main(void) {
  configure_pins();
  configure_rfm();
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

void SPI_send_byte(uint8_t addr, uint8_t data) {
  set_zero(portB, 3);
  
  while(!(*SPI_SR & (1 << 1)));
  *SPI_DR = (addr | 1);
  while(!(*SPI_SR & (1 << 1)) || !(*SPI_SR & (1 << 7)));
}

uint8_t SPI_read_byte(uint8_t addr) {
  set_zero(portB, 3);
  
  while(!(*SPI_SR & (1 << 2)));
  *SPI_DR = (addr);
  while(!(*SPI_SR & (1 << 2)));
  uint8_t data = *SPI_DR;
  while(!(*SPI_SR & (1 << 1)) || !(*SPI_SR & (1 << 7)));
  return data;
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
  *SPI_CR1 &= ~(7 << 3);    // f/2 baud rate mode
  *SPI_CR1 &= ~3;           // CPOL and CPHA conf
  set_zero(SPI_CR1, 7);     // MSB First (RFM requires that)
  *SPI_CR1 |= 3;            // software slave select & master mode
  set_one(SPI_CR1, 1 << 6); // SPI enable
  set_one(SPI_CR1, 1 << 2); // Master mode
  
  __asm("rim");
}

void configure_rfm(void) {
  uint8_t cur_mode = SPI_read_byte(RFM_MODE);
  cur_mode |= 0x03 << 2;
  SPI_send_byte(RFM_MODE, cur_mode);
  
  
  uint32_t frf = (217UL << 16); // 868 MHz
  uint8_t frfMsb = (uint8_t)((frf >> 16) & 0xFF);
  uint8_t frfMid = (uint8_t)((frf >> 8) & 0xFF);
  uint8_t frfLsb = (uint8_t)(frf & 0xFF);
  SPI_send_byte(FRF_MSB, frfMsb);
  SPI_send_byte(FRF_MID, frfMid);
  SPI_send_byte(FRF_LSB, frfLsb);
  
  
  cur_mode = SPI_read_byte(PCT_CFG);
  cur_mode |= (1 << 7); // Variable packet size
  SPI_send_byte(PCT_CFG, cur_mode);
}

#pragma vector = 12
__interrupt void EXTI2_Handler(void) {
  SPI_send_byte(RFM_FIFO, 2);
  /*if (is_pressed(2)) {
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
  }*/
  set_one(EXTI_SR1, 2);
}

#pragma vector = 13
__interrupt void EXTI3_Handler(void) {
  SPI_send_byte(RFM_FIFO, 3);
  /*if (is_pressed(3)) {
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
  }*/
  set_one(EXTI_SR1, 3);
}

#pragma vector = 14
__interrupt void EXTI4_Handler(void) {
  SPI_send_byte(RFM_FIFO, 4);
  /*if (is_pressed(4)) {
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
  }*/
  set_one(EXTI_SR1, 4);
}

#pragma vector = 15
__interrupt void EXTI5_Handler(void) {
  SPI_send_byte(RFM_FIFO, 5);
  /*if (is_pressed(5)) {
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
  }*/
  set_one(EXTI_SR1, 5);
}
