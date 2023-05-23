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