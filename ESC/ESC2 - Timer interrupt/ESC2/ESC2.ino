int i = 1;
int t = 1;
int on = 300;
int off = 1;
bool boo = true;

void setup() {

  Serial.begin(9600);
  Serial.println("Start");
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
  


  //TIMER
  //reset timer1 control register A
  TCCR1A = 0;

  //set prescaler to 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  //reset timer 1 count
  TCNT1 = 0;
  //set trigger count number
  OCR1A = 100;
  //Disable timer inturrupt
  TIMSK1 &= ~(1 << OCIE1A);

  

  next_phase();
  delay(2000);
  PORTB = 0;

  //Enable comparator interrupt flag
  //ACSR |= (1 << ACIE);

  //Enable timer inturrupt
  TIMSK1 = (1 << OCIE1A);

  //enable global inturrupts
  sei();
}

void loop() {

  
  }

ISR(ANALOG_COMP_vect) {  
  //reset the comparator intterupt flag
  next_phase();
  Serial.print(i);
  Serial.println(" ");
  ACSR &= ~(1 << ACI);
}

ISR(TIMER1_COMPA_vect)
{
  /*
  if(boo == true)
  {
    next_phase();
    OCR1A = on;
    boo == false;
  }
  else
  {
    
    PORTB = 0;
    OCR1A = off;
    boo = true;
  }
  */
  next_phase();
  
  //reset count
  TCNT1 = 0;
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
  //Serial.print(i);
  //Serial.print("  ");
  //Serial.println(PORTB, BIN);
  i %= 6;
  i++;
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
