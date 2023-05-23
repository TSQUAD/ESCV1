ISR(ANALOG_COMP_vect) {
  /*
  if((((ACSR) >> (ACO)) & 1)){
    PORTD |= (1 << PD2);
    ACSR &= ~(1 << ACIE);  
  }
  if(!(((ACSR) >> (ACO)) & 1)){
    PORTD &= ~(1 << PD2);
    ACSR &= ~(1 << ACIE);  
  }
  */
  if((((ACSR) >> (ACO)) & 1)){
    PORTD |= (1 << PD2); 
  }
  else{
    ACSR &= ~(1 << ACIE);  
  }
  
  //clear inturrupt flag
  //ACSR &= ~(1 << ACI);
}


//(((value) >> (bit)) & 0x01)
