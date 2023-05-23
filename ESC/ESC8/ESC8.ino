int i = 0;
int k = 0;
int p = 0;

int PWM_WIDTH = 200;
int end = 190;

int RPM = 1000;
int tick  = 255;
int max_RPM = 7000;

//OC0A - PD6 - 6 AH <- not useful
//OC0B - PD5 - 5 BH
//OC2A - PB3 - 11 CH
//OC2B - PD3 - 3 AH
//PB0 8 CL
//PB1 9 BL
//PB2 10 AL


//BEMF
//PD6 - D6 - Comparator Zero PD6

//ADC0 = Phase A
//ADC1 = Phase B
//ADC2 = Phase C



void setup() {
  PORTD = 0;
  DDRD |= (1 << PD5);
  DDRB |= (1 << PB3);
  DDRD |= (1 << PD3);
  
  DDRB |= (1 << PB0);
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);

  Serial.begin(9600);
  Serial.println("Begin");

  // put your setup code here, to run once:
  

  TIMER0_SET_UP();
  TIMER1_SET_UP();
  TIMER2_SET_UP();
  //MUX_SET_UP();
  calculate_RPM();
  
  sei();
}

void loop() {
  Serial.println(RPM);
}

  //     SETUP      //
  //   TIMER 0   //
void TIMER0_SET_UP(){
  //Reset TimerA 0 interrupt
  TCCR0A = 0;
  //Reset TimerB 0 interrupt
  TCCR0B = 0;

  //FAST PWM Mode
  TCCR0A |= (1 << WGM01);
  TCCR0A |= (1 << WGM00);

  //Clear OC0A on compare match (to 0)
  //TCCR0A |= (1 << COM0A1);
  //Clear OC0B on compare match (to 0)
  TCCR0A |= (1 << COM0B1);

  //Enable interrupt on OCR1A
  //TIMSK0 |= (1 << OCIE0A);

  //Prescaler 1
  TCCR0B |= (1 << CS20);

  //Timer 0 count A
  OCR0A = PWM_WIDTH; //off time
  //Timer 0 count B
  OCR0B = PWM_WIDTH;  
}
 
  //   TIMER 1   //
void TIMER1_SET_UP(){
  //Reset TimerA 1 interrupt
  TCCR1A = 0;
  //Reset TimerB 1 interrupt
  TCCR1B = 0;

  //CTC mode
  TCCR1A |= (1 << WGM12);
  
  //Prescale 256
  TCCR1B |= (1 << CS12);
  //TCCR1B |= (1 << CS11);
  //TCCR1B |= (1 << CS10);

  //Enable interrupt on OCR1A
  TIMSK1 |= (1 << OCIE1A);
  //Enable interrupt on OCR1B
  TIMSK1 |= (1 << OCIE1B);

  //Update timer counter
  OCR1A = 1000;
  OCR1B = 4;
}
  //   TIMER 0   //
void TIMER2_SET_UP(){
  //Reset TimerA 0 interrupt
  TCCR2A = 0;
  //Reset TimerB 0 interrupt
  TCCR2B = 0;

  //FAST PWM Mode
  TCCR2A |= (1 << WGM01);
  TCCR2A |= (1 << WGM00);

  //Clear OC0A on compare match (to 0) PIN 11
  TCCR2A |= (1 << COM2A1);
  //Clear OC0B on compare match (to 0)
  TCCR2A |= (1 << COM2B1);

  //Prescaler 1
  //TCCR2B |= (1 << CS22);
  //TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS20);

  //Timer 0 count A
  OCR2A = PWM_WIDTH; //off time
  //Timer 0 count B
  OCR2B = PWM_WIDTH;  
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
  ACSR |= (1 << ACIS0);
  ACSR |= (1 << ACIS1);

  //Enable comparator interrupt flag
  //ACSR |= (1 << ACIE);  

  //clear inturrupt flag
  ACSR &= ~(1 << ACI);
}

ISR(ANALOG_COMP_vect) {  
  //reset the comparator intterupt flag
  p++;
  p %= 6;
  if(i==0){
    i=p;
  }
  /*i++;
  phase();
  i %= 6;
  TCNT1 = 0;
  */
  if(i%2){
    ACSR &= ~(1 << ACIS0);
  }
  else  {
    ACSR |= (1 << ACIS0);
  }
  ACSR &= ~(1 << ACI);
}

  //   TIMER 1 A  //
ISR(TIMER1_COMPA_vect){
  i++;
  phase();
  i %= 6;
  TCNT1 = 0;
}
//Motor Calcs
void calculate_RPM(){
  tick = (double)(pow(RPM,-1)*(16*pow(10,6)*60)/(256*6*2));
  OCR1A = tick;
}

ISR(TIMER1_COMPB_vect){
  if(i%6 == 0){
    if(RPM < max_RPM){
      RPM += 10;
      calculate_RPM();
    }
    else if(end > 115)
    {
      k++;
      if(k%17 == 0){
        k = 0;
        end--;
        OCR2A = end;
        OCR0B = end;
        OCR2B = end;
      }
    }
    else{
      OCR2A = end;
      OCR0B = end;
      OCR2B = end;
      TIMSK1 &= ~(1 << OCIE1B);
    }  
  }
}
ISR(TIMER0_COMPA_vect){
}

//OC0A - PD6 - 6 AH
//OC0B - PD5 - 5 BH
//OC2A - PB3 - 11 CH
//PB0 8 CL
//PB1 9 BL
//PB2 10 AL

void phase(){  
  switch(i){ 
    case 1:
      TCCR2A &= ~(1 << COM2A1);
      ADMUX = 0b010;
      TCCR2A |= (1 << COM2B1);
      break;      
    case 2:
      PORTB |= (1 << PB0);
      ADMUX = 0b001;
      PORTB &= ~(1 << PB1);
      break;
    case 3:
      TCCR2A &= ~(1 << COM2B1);
      ADMUX = 0b000;
      TCCR0A |= (1 << COM0B1);
      break;
    case 4:
      PORTB |= (1 << PB2);
      ADMUX = 0b010;
      PORTB &= ~(1 << PB0);
      break;
    case 5:
      TCCR0A &= ~(1 << COM0B1);
      ADMUX = 0b001;
      TCCR2A |= (1 << COM2A1);
      break;
    case 6:
      PORTB |= (1 << PB1);
      ADMUX = 0b000;
      PORTB &= ~(1 << PB2);
      break;
  }
}