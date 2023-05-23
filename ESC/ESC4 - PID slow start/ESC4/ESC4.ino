//Phase
int i = 0;

//Motor characteristics
int RPM = 1500;
//Number of ticks per cycle
int tick = 6250;

//clock prescale values
int prescale_0 = 256;
int prescale_1 = 256;

float time_per_phase = 0;

void setup() {
  //   MOTOR SETUP   //
  calculate_RPM();
    
  /*Serial.begin(9600);
  Serial.print("Ticks: ");
  Serial.println(tick);
  */
  //disable global interrupts
  cli(); 

  //Set PORTS to be output and disable
  PORTB_SET_UP();
  //Disable ADC and setup MUX
  //MUX_SET_UP();

  //Timer Setups
  //PWM Driver
  TIMER0_SET_UP();
  //Phase Driver
  TIMER1_SET_UP();
  //PID Controller
  TIMER2_SET_UP();

  //enable global interrupts
  sei();  
}

void loop() 
{

}

  //     SETUP      //
  //   TIMER 0   //
void TIMER0_SET_UP(){
  //Reset TimerA 0 interrupt
  TCCR0A = 0;
  //Reset TimerB 0 interrupt
  TCCR0B = 0;

  //CTC Mode
  TCCR0A |= (1 << WGM01);

  //enable interrupt for OCR0A
  TIMSK0 |= (1 << OCIE0A);
  //enable interrupt for OCR0B
  //TIMSK0 |= (1 << OCIE0B);

  //Prescaler setup for timer 0
  if(prescale_0 == 256){
    TCCR0B |= (1 << CS12);
    Serial.println("Prescale 0: 256");
  }
  else if(prescale_0 == 64){
    TCCR0B |= (1 << CS11);
    TCCR0B |= (1 << CS10);
    Serial.println("Prescale 0: 64");
  }
  else if(prescale_0 == 8){
    TCCR0B |= (1 << CS11);
    Serial.println("Prescale 0: 8");
  }
  else{
    //1024 prescale
    TCCR0B |= (1 << CS12);
    TCCR0B |= (1 << CS10);
    Serial.println("Prescale 0: 1024");
  }
  
  //Timer 0 count A (Period ticks)
  OCR0A = 20;
  //Timer 0 count B (off ticks)
  OCR0B = 1;  
}

  //   TIMER 1   //
void TIMER1_SET_UP(){
  //Reset TimerA 1 interrupt
  TCCR1A = 0;
  //Reset TimerB 1 interrupt
  TCCR1B = 0;

  //CTC mode
  TCCR1A |= (1 << WGM12);

  //Prescaler setup for timer 1
  if(prescale_1 == 256){
    TCCR1B |= (1 << CS12);
    Serial.println("Prescale 1: 256");
  }
  else if(prescale_1 == 64){
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS11);
    Serial.println("Prescale 1: 64");
  }
  else{

    //1024 prescale
    TCCR1B |= (1 << CS12);
    TCCR1B |= (1 << CS10);
    Serial.println("Prescale 1: 1024");
  }

  //Enable interrupt on OCR1A
  TIMSK1 |= (1 << OCIE1A);
  //Enable interrupt on OCR1B
  //TIMSK1 |= (1 << OCIE1B);

  //Update timer counter
  OCR1A = tick;
  //OCR1B = 1;
}

  //   TIMER 2   //
void TIMER2_SET_UP(){
  //reset timer 2 registers
  TCCR2A = 0;
  TCCR2B = 0;

  //CTC mode
  TCCR2B |= (1 << WGM12);

  //1024 prescaler
  TCCR2B |= (1 << CS22);
  TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS20);

  //Enable timer coutner  
  TIMSK2 |= (1 << OCIE2A);

  //timer ticks
  OCR2A = 5;
}

  //  ADC SETUP  //
void MUX_SET_UP(){
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

  //Enable comparator interrupt flag
  //ACSR |= (1 << ACIE);  

  //clear inturrupt flag
  ACSR &= ~(1 << ACI);
}

  //  PORT B SETUP  //
void PORTB_SET_UP(){
  //PORT SETUP
  PORTB = 0;
  //pin 13 output (AH)
  DDRB |= (1 << PB5);
  //pin 12 output (AL)
  DDRB |= (1 << PB4);
  //pin 11 output (BH)
  DDRB |= (1 << PB3);
  //pin 10 output (BL)
  DDRB |= (1 << PB2);
  //pin 9 output (CH)
  DDRB |= (1 << PB1);
  //pin 8 output (CL)
  DDRB |= (1 << PB0);
}


//   INTERRUPT SERVICE ROUTINES   //

  //   TIMER 0 A  //
ISR(TIMER0_COMPA_vect){
  phase();
  TCNT0 = 0;
}
  //   TIMER 0 B   //
//Timer to update everything
ISR(TIMER0_COMPB_vect){
  PORTB = 0;
}

  //   TIMER 1 A  //
ISR(TIMER1_COMPA_vect){
  //Change Phase
  i++;
  i %= 6;
  //reset count
  TCNT1 = 0;
}

  //   TIMER 1 B  //
/*
ISR(TIMER1_COMPB_vect){  
  Serial.println(i);
}
*/

  //   TIMER 2 A   //
ISR(TIMER2_COMPA_vect){
  if(RPM < 3000){
    RPM++;
    OCR1A = tick;
    calculate_RPM();
  }  
  else
  {
    TIMSK2 &= ~(1 << OCIE2A);
  }
}

/*Not ready for analog comparator ISR yet 
ISR(ANALOG_COMP_vect) {  
  //reset the comparator intterupt flag
  next_phase();
  ACSR &= ~(1 << ACI);
}
*/

//Motor Calcs
void calculate_RPM(){

  //Number of phases * Pole pairs / 60 seconds
  time_per_phase = RPM*6*2/60;
  //1 phase  = x seconds
  time_per_phase = pow(time_per_phase,-1);

  //Number of ticks to make one phase
  tick = time_per_phase/(prescale_1/(16*pow(10,6)));
} 
/*
void status(){
  Serial.print("RPM: ");
  Serial.print(RPM);
  Serial.print(" Phase: ");
  Serial.print(i);
  Serial.print(" Ticks: ");
  Serial.println(tick);
}
*/


void phase(){  
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

//Pin 13 - PB5 - AH
//Pin 12 - PB4 - AL
//Pin 11 - PB3 - BH
//Pin 10 - PB2 - BL
//Pin 9 - PB1 - CH
//Pin 8 - PB0 - CL

//0 - 1 set AH and BL
void AH_BL() {
  PORTB = 0b100100;
}
//60 - 2 set AH and CL
void AH_CL(){
  PORTB = 0b100001;
}
//120 - 3 set AH and CL
void BH_CL(){
  PORTB = 0b001001;
}
//180 - 4 set AH and CL
void BH_AL(){
  PORTB = 0b011000;
}
//240 - 5 set AH and CL
void CH_AL(){
  PORTB = 0b010010;
}
//300 -6 set AH and CL
void CH_BL(){
  PORTB = 0b000110;
}
//360 - 0 RESTART
