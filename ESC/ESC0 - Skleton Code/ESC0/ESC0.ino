int t = 10;
void setup() {
  // put your setup code here, to run once:
  DDRB |= (1 << PB5);
  PORTB &= ~(1 << PB5);

}

void loop() {
  // put your main code here, to run repeatedly:
  PORTB = 0b100100; 
  delay(t);
  PORTB = 0b100001;
  delay(t);
  PORTB = 0b001001;
  delay(t);
  PORTB = 0b011000;
  delay(t);
  PORTB = 0b010010;
  delay(t); 
  PORTB = 0b000110;
  delay(t);
}


