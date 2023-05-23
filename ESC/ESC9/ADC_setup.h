  //  ADC SETUP  //
void MUX_SET_UP(){
  // Disable the ADC module
  ADCSRA = (0 << ADEN);
  // MUX select for negative input of comparator
  ADCSRB |= (1 << ACME);
  // Select A0 as comparator negative input
  ADMUX = 0b000;


  //ACIS0,ACIS1 represent rising or falling edge (It's checking for a change)
  //ACSR |= (1 << ACIS0);
  ACSR |= (1 << ACIS1);

  //Enable comparator interrupt flag
  //ACSR |= (1 << ACIE);  

  //clear inturrupt flag
  ACSR &= ~(1 << ACI);
}


