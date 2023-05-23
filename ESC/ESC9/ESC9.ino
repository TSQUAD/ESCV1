#include "main_parameters.h"
#include "timing_logic.h"
#include "port_setup.h"
#include "timer_setup.h"
#include "ADC_setup.h"
#include "timer_ISR.h"
#include "ADC_ISR.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
  
  port_setup();
  TIMER0_SET_UP();
  TIMER1_SET_UP();
  TIMER2_SET_UP();
  //MUX_SET_UP();
  calculate_RPM();
  
  //enable global interrupts
  sei();
  while(RPM<max_RPM){
    Serial.println(RPM);
  }

}
void loop(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.parseInt();
    if(incomingByte != 0){
      OCR2A = incomingByte;
      OCR0B = incomingByte;
      OCR2B = incomingByte;
      Serial.println(OCR2A,DEC);  
    }    
  }
  if((((ACSR) >> (ACO)) & 1)){
    PORTB |= (1 << PB4);
  }
  else{
    PORTB &= ~(1 << PB4);
  }
  
}