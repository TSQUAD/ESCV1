//Phase
int i = 0;

//Motor characteristics
int RPM = 500;
int max_RPM = 1000;
//Number of ticks per cycle
int tick = 6250;

// timer intilization //
//T0 ticks
int t0a = 100;
int t0b = 10;
//T2 ticks
int t2a = 50;
int t2b = 5;


//clock prescale values
int prescale_0 = 1024;
int prescale_1 = 256;
int prescale_2 = 256;

float time_per_phase = 0;

//duty cycle
double DC = 0;
bool a = false;

void setup() {
  //disable global interrupts
  cli(); 
  //   MOTOR SETUP   //
  calculate_RPM();

  Serial.begin(9600);
  status();
  
  //Set PORTS to be output and disable
  PORTB_SET_UP();
  //Disable ADC and setup MUX
  //MUX_SET_UP();

  //Timer Setups
  //PID Controller
  TIMER0_SET_UP();
  //Phase Driver
  TIMER1_SET_UP();
  //PWM Controller
  TIMER2_SET_UP();

  //enable global interrupts
  sei();  
}

void loop(){
  if(!a){
    Serial.println(DC);
  }
  
}

void stat(){
  Serial.print("RPM");
  Serial.print(RPM);
  Serial.print("tick");
  Serial.println(tick);  
}

void status(){
  Serial.print("RPM: ");
  Serial.print(RPM);
  Serial.print(" Phase: ");
  Serial.print(i);
  Serial.print(" T1a: ");
  Serial.println(tick);
  Serial.print("T0a: ");
  Serial.print(t0a);
  Serial.print(" T2a: ");
  Serial.print(t2a);
  Serial.print(" T2b: ");
  Serial.println(t2b);
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
  
  //Timer 0 count A
  OCR0A = t0a;
  //Timer 0 count B
  OCR0B = t0b;  
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
  TIMSK1 |= (1 << OCIE1B);

  //Update timer counter
  OCR1A = tick;
  OCR1B = tick/2;
}

  //   TIMER 2   //
void TIMER2_SET_UP(){
  //reset timer 2 registers
  TCCR2A = 0;
  TCCR2B = 0;

  //CTC mode
  TCCR2B |= (1 << WGM12);

  if(prescale_2 == 256){
    TCCR2B |= (1 << CS22);
    TCCR2B |= (1 << CS20);
    Serial.println("Prescale 2: 256");
  }
  else if(prescale_2 == 64){
    TCCR2B |= (1 << CS22);
    Serial.println("Prescale 2: 64");
  }
  else if(prescale_2 == 8){
    TCCR2B |= (1 << CS21);
    Serial.println("Prescale 2: 8");
  }
  else{
    //1024 prescale
    TCCR2B |= (1 << CS22);
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS20);
    Serial.println("Prescale 2: 1024");
  }
  //Enable timer 1 A coutner  
  TIMSK2 |= (1 << OCIE2A);
  //Enable timer 1 B coutner  
  //TIMSK2 |= (1 << OCIE2B);

  //timer ticks
  OCR2A = t2a;
  //timer ticks
  OCR2B = t2b;
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
  if(RPM < max_RPM){
    RPM++;
    calculate_RPM();
    OCR1A = tick;
    OCR1B = tick/2;
  }  
  else
  {
    status();
    TIMSK2 |= (1 << OCIE2B);
    TIMSK0 &= ~(1 << OCIE0A);
  }
  TCNT0 = 0;
}
  //   TIMER 0 B   //
//Timer to update everything
ISR(TIMER0_COMPB_vect){
}

  //   TIMER 1 A  //
ISR(TIMER1_COMPA_vect){
  
  //Change Phase
  i %= 6;
  if(i == 0)
  {    
    TCNT2 = 0;
    TCNT0 = 0;
    a = 1;
  }
  
  i++;
  a = !a;  
  
  //reset count
  TCNT1 = 0;
  //TCNT2 = 0;
}
  //   TIMER 1 B  //

ISR(TIMER1_COMPB_vect){
  OCR1B = tick/2;
}

  //   TIMER 2 A   //
ISR(TIMER2_COMPA_vect){
  phase();

  DC = (double)(TCNT1)/(double)OCR1A;
  if(a)
  {
    DC = 1-DC;
  }
  
  OCR2B = OCR2A*DC;

  //reset count
  TCNT2 = 0;
}
  //   TIMER 2 B   //
ISR(TIMER2_COMPB_vect){
  PORTB = 0;
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


void phase(){  
  switch(i){
    case 1:
      AH_BL();
      break;
    case 2:
      AH_CL();
      break;
    case 3:
      BH_CL();
      break;
    case 4:
      BH_AL();
      break;
    case 5:
      CH_AL();
      break;
    case 6:
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
