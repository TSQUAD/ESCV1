int i = 0;
int r = 1000;
int r2 = 800;
void setup() {

  Serial.begin(9600);
  //Serial.println("Start");

  //disable global interrupts
  cli(); 

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
  




  

  //TIMER 1

  //Reset TimerA 1 interrupt
  TCCR1A = 0;
  //Reset TimerB 1 interrupt
  TCCR1B = 0;

  //CTC mode
  TCCR1A |= (1 << WGM12);
  //Prescaler at 256
  TCCR1B |= (1 << CS12);

  //Enable interrupt on OCR1A
  TIMSK1 |= (1 << OCIE1A);
  //Enable interrupt on OCR1B
  TIMSK1 |= (1 << OCIE1B);

  //Update timer counter
  OCR1A = 500;
  OCR1B = 50;



  //TIMER 0

  //Reset TimerA 0 interrupt
  TCCR0A = 0;
  //Reset TimerB 0 interrupt
  TCCR0B = 0;

  //CTC Mode
  TCCR0A |= (1 << WGM01);

  //enable interrupt for OCR0A
  TIMSK0 |= (1 << OCIE0A);
  //enable interrupt for OCR0B
  TIMSK0 |= (1 << OCIE0B);

  //64  prescaler
  TCCR0B |= (1 << CS11);
  TCCR0B |= (1 << CS10);
  
  //Timer 0 count A
  OCR0A = 10;
  //Timer 0 count B
  OCR0B = 10;

  //enable global interrupts
  sei();  
     

  //Enable comparator interrupt flag
  //ACSR |= (1 << ACIE);  
}

void loop() 
{

}
/*Not ready for analog comparator ISR yet 
ISR(ANALOG_COMP_vect) {  
  //reset the comparator intterupt flag
  next_phase();
  ACSR &= ~(1 << ACI);
}
*/

//PWM for the thing
ISR(TIMER0_COMPA_vect)
{
  phase();
  TCNT0 = 0;
}
//Timer to update everything
ISR(TIMER0_COMPB_vect)
{
  PORTB = 0;
}


//Move to the next phase
ISR(TIMER1_COMPA_vect)
{
  i %= 6;
  i++;
  
  //reset count
  TCNT1 = 0;
  
}


 //this is being commented out because we just need to know when to call the next phase
ISR(TIMER1_COMPB_vect)
{  
  //Serial.println(i);
}


void phase()
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
  //ADMUX = 0b001;
  //set to trigger on falling edge
  //ACSR &= ~(1 << ACIS0);
}
//60 - 2
void AH_CL()
{
  //set AH and CL
  PORTB = 0b100001;
  //set phase A (ADC0) to be negitive input
  //ADMUX = 0b000;
  //set to trigger on rising edge
  //ACSR |= (1 << ACIS0);
}
//120 - 3
void BH_CL()
{
  //set AH and CL
  PORTB = 0b001001;
  //set phase C (ADC2) to be negitive input
  //ADMUX = 0b010;
  //set to trigger on falling edge
  //ACSR &= ~(1 << ACIS0);
}
//180 - 4
void BH_AL()
{
  //set AH and CL
  PORTB = 0b011000;
  //set phase B (ADC1) to be negitive input
  //ADMUX = 0b010;
  //set to trigger on rising edge
  //ACSR |= (1 << ACIS0);
}
//240 - 5
void CH_AL()
{
  //set AH and CL
  PORTB = 0b010010;
  //set phase A (ADC0) to be negitive input
  //ADMUX = 0b000;
  //set to trigger on falling edge
  //ACSR &= ~(1 << ACIS0);
}
//300 -6
void CH_BL()
{
  //set AH and CL
  PORTB = 0b000110;
  //set phase C (ADC2) to be negitive input
  //ADMUX = 0b010;
  //set to trigger on rising edge
  //ACSR |= (1 << ACIS0);
}
//360 - 0
