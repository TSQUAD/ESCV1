void port_setup(){
  PORTD = 0;
  DDRD |= (1 << PD5);
  DDRB |= (1 << PB3);
  DDRD |= (1 << PD3);
  
  DDRB |= (1 << PB0);
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);
  
  //devloper pins
  DDRD |= (1 << PD2);
  DDRB |= (1 << PB4);
}
//OC0A - PD6 - 6 AH <- not useful
//OC0B - PD5 - 5 BH
//OC2A - PB3 - 11 CH
//OC2B - PD3 - 3 AH
//PB0 8 CL
//PB1 9 BL
//PB2 10 AL

//BEMF
//PD6 - D6 - Comparator Zero PD6

//ADC0 = Phase A 0b000
//ADC1 = Phase B 0b001
//ADC2 = Phase C 0b010