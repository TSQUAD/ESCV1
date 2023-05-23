//Phase
int i = 1;

//Motor characteristics
int RPM = 1000;
int max_RPM = 5500;
//Number of ticks per cycle
int tick = 150;

int t0a = 50;
int t0b = 10;

int t2a = 200;
int t2b = 13; 

//clock prescale values
int prescale_0 = 256;
int prescale_1 = 256;
int prescale_2 = 256;


void setup() {
  //disable global interrupts
  cli(); 

  Serial.begin(9600);

  calculate_RPM();
  Serial.println(tick);
  
  //Set PORTS to be output and disable
  PORTB_SET_UP();
  HSH();
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
  while(RPM < 1550)
  {
    if(RPM > 1525)
    {
      OCR2A = 225;
      break;
    }  
  }
  while(RPM < 2925)
  {
    if(RPM > 2910)
    {
      OCR2A = 176;
      break;
    }  
  }
}

void loop(){
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
  //TIMSK0 |= (1 << OCIE0A);
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
  OCR1A = 100;
  OCR1B = 50;
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
  TIMSK2 |= (1 << OCIE2B);

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
  HSH();   
  TCNT0 = 0;
}
  //   TIMER 0 B   //
//Timer to update everything
ISR(TIMER0_COMPB_vect){
  HSL();
}

  //   TIMER 1 A  //
ISR(TIMER1_COMPA_vect){
  i++;
  LS();
  TCNT1 = 0;
}
  //   TIMER 1 B  //
ISR(TIMER1_COMPB_vect){
  if(i>=6){
    i = 0;
    TCNT0 = 0;
    TCNT2 = 0;     
  }
  i++;
  LS();
}

  //   TIMER 2 A   //
ISR(TIMER2_COMPA_vect){
  if(RPM > max_RPM){
    //TIMSK0 |= (1 << OCIE0A);
    //TIMSK0 |= (1 << OCIE0B);
    TIMSK2 &= ~(1 << OCIE2A);
    return;
  }
  else{
    RPM++;
  }
  TCNT2 = 0;
}
  //   TIMER 2 B   //
ISR(TIMER2_COMPB_vect){
  calculate_RPM();
}
//   Not ready for analog comparator ISR yet   //
ISR(ANALOG_COMP_vect) {  
  //reset the comparator intterupt flag
  //next_phase();
  ACSR &= ~(1 << ACI);
}

//Motor Calcs
void calculate_RPM(){
  tick = pow(RPM*6*2/60,-1)/(prescale_1/(16*pow(10,6)));
  OCR1A = tick*2;
  OCR1B = tick;
}

//Pin 13 - PB5 - AH
//Pin 12 - PB4 - AL
//Pin 11 - PB3 - BH
//Pin 10 - PB2 - BL
//Pin 9 - PB1 - CH
//Pin 8 - PB0 - CL

void LS(){  
  switch(i){
    case 1:  
      PORTB &= ~(1 << PB1);
      PORTB |= (1 << PB5);
      break;      
    case 2:
      PORTB |= (1 << PB0);
      PORTB &= ~(1 << PB2);
      break;
    case 3:
      PORTB &= ~(1 << PB5);
      PORTB |= (1 << PB3);
      break;
    case 4:
      PORTB |= (1 << PB4);
      PORTB &= ~(1 << PB0);
      break;
    case 5:
      PORTB &= ~(1 << PB3);
      PORTB |= (1 << PB1);
      break;
    case 6:
      PORTB |= (1 << PB2);
      PORTB &= ~(1 << PB4);
      break;
  }
}
void HSH(){  
  switch(i){
    case 1: 
      PORTB |= (1 << PB5);
      break;
    case 2:
      PORTB |= (1 << PB5);
      break; 
    case 3:
      PORTB |= (1 << PB3);
      break;
    case 4:
      PORTB |= (1 << PB3);
      break;    
    case 5:
      PORTB |= (1 << PB1);
      break;
    case 6:
      PORTB |= (1 << PB1);
      break;
  }
}
void HSL(){  
  switch(i){
    case 1: 
      PORTB &= ~(1 << PB5);
      break;
    case 2:
      PORTB &= ~(1 << PB5);
      break; 
    case 3:
      PORTB &= ~(1 << PB3);
      break; 
    case 4:
      PORTB &= ~(1 << PB3);
      break;    
    case 5:
      PORTB &= ~(1 << PB1);
      break;
    case 6:
      PORTB &= ~(1 << PB1);
      break;
  }
}