void A_rising(){
  ACSR |= (1 << ACIS0);
  ADMUX = 0b000;
}
void A_falling(){
  ACSR &= ~(1 << ACIS0);
  ADMUX = 0b000;
}
void B_rising(){
  ACSR |= (1 << ACIS0);
  ADMUX = 0b001;
}
void B_falling(){
  ACSR &= ~(1 << ACIS0);
  ADMUX = 0b001;
}
void C_rising(){
  ACSR |= (1 << ACIS0);
  ADMUX = 0b010;
}
void C_falling(){
  ACSR &= ~(1 << ACIS0);
  ADMUX = 0b010;  
}