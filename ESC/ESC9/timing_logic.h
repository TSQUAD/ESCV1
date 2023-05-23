#include "BEMF_MUX.h"

void phase(){  
  ADCSRA &= ~(1 << ADEN);
  ADCSRB |= (1 << ACME);  
  switch(i){ 
    case 1:
      TCCR2A &= ~(1 << COM2A1);
      C_rising();
      TCCR2A |= (1 << COM2B1);
      break;      
    case 2:
      PORTB |= (1 << PB0);
      B_falling();
      PORTB &= ~(1 << PB1);
      break;
    case 3:
      TCCR2A &= ~(1 << COM2B1);
      A_rising();
      TCCR0A |= (1 << COM0B1);
      break;
    case 4:
      PORTB |= (1 << PB2);
      C_falling();
      PORTB &= ~(1 << PB0);
      break;
    case 5:
      TCCR0A &= ~(1 << COM0B1);
      B_rising();
      TCCR2A |= (1 << COM2A1);
      break;
    case 6:
      PORTB |= (1 << PB1);
      A_falling();
      PORTB &= ~(1 << PB2);
      break;
  }
  //ACSR |= (1 << ACIE);  
}
//Motor Calcs
void calculate_RPM(){
  tick = (double)(pow(RPM,-1)*(16*pow(10,6)*60)/(256*6*2));
  OCR1A = tick;
}

