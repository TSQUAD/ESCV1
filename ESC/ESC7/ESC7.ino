int i = 0;
int PWM_WIDTH = 255;
int start = 100;
int max_RPM = 20;

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
  //DDRD |= (1 << PD6);
  DDRD |= (1 << PD5);
  DDRB |= (1 << PB3);
  DDRB |= (1 << PD3);
  
  DDRB |= (1 << PB0);
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);

  Serial.begin(9600);
  Serial.println("Begin");

  // put your setup code here, to run once:
  TIMER0_SET_UP();
  TIMER1_SET_UP();
  TIMER2_SET_UP();
  sei();
  int end = 85;
  while(OCR1A > 50){
    if(OCR1A < 55){
      OCR0A = end;
      OCR0B = end;
      OCR2A = end;
      break;
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
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
  TCCR1B |= (1 << CS10);

  //Enable interrupt on OCR1A
  TIMSK1 |= (1 << OCIE1A);
  //Enable interrupt on OCR1B
  TIMSK1 |= (1 << OCIE1B);

  //Update timer counter
  OCR1A = start;
  OCR1B = 3;
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
  TCCR2A |= (1 << COM2A1);

  //Enable interrupt on OCR1A
  //TIMSK2 |= (1 << OCIE0A);

  //Prescaler 1
  TCCR2B |= (1 << CS10);

  //Timer 0 count A
  OCR2A = PWM_WIDTH; //off time
  //Timer 0 count B
  OCR2B = PWM_WIDTH;  
}
  //   TIMER 1 A  //
ISR(TIMER1_COMPA_vect){
  i++;
  phase();
  i %= 6;
  TCNT1 = 0;
}
ISR(TIMER1_COMPB_vect){
  if(i%6 == 0){
    if(OCR1A > max_RPM){
      OCR1A--;    
    }
    else{
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
//Pin 6 - PB5 - AH
//Pin 10 - PB4 - AL
//Pin 5 - PB3 - BH
//Pin 9 - PB2 - BL
//Pin 11 - PB1 - CH
//Pin 8 - PB0 - CL

void phase(){  
  switch(i){ 
    case 1:
      TCCR2A &= ~(1 << COM2A1);
      TCCR2A |= (1 << COM2A1);
      break;      
    case 2:
      PORTB |= (1 << PB0);
      PORTB &= ~(1 << PB1);
      break;
    case 3:
      TCCR2A &= ~(1 << COM2A1);
      TCCR0A |= (1 << COM0B1);
      break;
    case 4:
      PORTB |= (1 << PB2);
      PORTB &= ~(1 << PB0);
      break;
    case 5:
      TCCR0A &= ~(1 << COM0B1);
      TCCR2A |= (1 << COM2A1);
      break;
    case 6:
      PORTB |= (1 << PB1);
      PORTB &= ~(1 << PB2);
      break;
  }
}