int i = 1;
int t = 1;

void setup() {

  Serial.begin(9600);
  Serial.println("it works");
  //PORT SETUP
  //pin 13 output (AH)
  DDRB |= (1 << PB5);
  PORTB &= ~(1 << PB5);
  //pin 12 output (AL)
  DDRB |= (1 << PB4);
  PORTB &= ~(1 << PB4);;
  //pin 11 output (BH)
  DDRB |= (1 << PB3);
  PORTB &= ~(1 << PB3);
  //pin 10 output (BL)
  DDRB |= (1 << PB2);
  PORTB &= ~(1 << PB2);
  //pin 9 output (CH)
  DDRB |= (1 << PB1);
  PORTB &= ~(1 << PB1);
  //pin 8 output (CL)
  DDRB |= (1 << PB0);
  PORTB &= ~(1 << PB0);

  //ADC SETUP
  // Disable the ADC module
  ADCSRA = (0 << ADEN);
  // MUX select for negative input of comparator
  ADCSRB |= (1 << ACME);
  // Select A0 as comparator negative input
  ADMUX = 0b000;
  //ADC0 = Phase A 0b000
  //ADC1 = Phase B 0b001
  //ADC2 = Phase C 0b010

  //ACIS0,ACIS1 represent rising or falling edge (It's checking for a change)
  ACSR &= ~(1 << ACIS0);
  ACSR |= (1 << ACIS1);

  //clear inturrupt flag
  ACSR &= ~(1 << ACI);
  //enable interrupt flag
  //ACSR |= (1 << ACIE);


  next_phase();
  delay(500);
  PORTB = 0;
}

void loop() {
  
  next_phase();
  i++;
  
  Serial.print(i);
  Serial.print("  ");
  Serial.println(PORTB, BIN);
  i %= 6;
  }

ISR(ANALOG_COMP_vect) {  
  //reset the comparator intterupt flag
  i++;
  next_phase();
  Serial.println("interrupt");
  //reset comparator flag
  ACSR &= ~(1 << ACI);
}



void next_phase()
{
  switch(i){
    case 0:
      AH_BL();
      break;
    case 1:
      AH_CL();
      break;
    case 2:
      BH_CL();
      break;
    case 3:
      BH_AL();
      break;
    case 4:
      CH_AL();
      break;
    case 5:
      CH_BL();
      break;
  }
}

//0 - 1
void AH_BL() {
  //set AH and BL
  PORTB = 0b100100;
  //set phase B (ADC1) to be negitive input
  ADMUX = 0b001;
  //set to trigger on falling edge
  ACSR &= ~(1 << ACIS0);
}
//60 - 2
void AH_CL()
{
  //set AH and CL
  PORTB = 0b100001;
  //set phase A (ADC0) to be negitive input
  ADMUX = 0b000;
  //set to trigger on rising edge
  ACSR |= (1 << ACIS0);
}
//120 - 3
void BH_CL()
{
  //set AH and CL
  PORTB = 0b001001;
  //set phase C (ADC2) to be negitive input
  ADMUX = 0b010;
  //set to trigger on falling edge
  ACSR &= ~(1 << ACIS0);
}
//180 - 4
void BH_AL()
{
  //set AH and CL
  PORTB = 0b011000;
  //set phase B (ADC1) to be negitive input
  ADMUX = 0b010;
  //set to trigger on rising edge
  ACSR |= (1 << ACIS0);
}
//240 - 5
void CH_AL()
{
  //set AH and CL
  PORTB = 0b010010;
  //set phase A (ADC0) to be negitive input
  ADMUX = 0b000;
  //set to trigger on falling edge
  ACSR &= ~(1 << ACIS0);
}
//300 -6
void CH_BL()
{
  //set AH and CL
  PORTB = 0b000110;
  //set phase C (ADC2) to be negitive input
  ADMUX = 0b010;
  //set to trigger on rising edge
  ACSR |= (1 << ACIS0);
}
//360 - 0
