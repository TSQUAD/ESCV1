  //   TIMER 1 A  //
ISR(TIMER1_COMPA_vect){
  i++;
  phase();
  i %= 6;
  TCNT1 = 0;
}

ISR(TIMER1_COMPB_vect){
  if(i%6 == 0){
    if(RPM < max_RPM){
      RPM += 10;
      calculate_RPM();
    }
    else if(end > end_final)
    {
      k++;
      if(k%21 == 0){
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
      //TIMSK1 &= ~(1 << OCIE1A);
      p = i;
      //ACSR |= (1 << ACIE);  
      
    }  
  }
}