void enableInterrupts() 
{
  cli(); // disable interrupts
  // reset
  TCCR1A = 0; // set TCCR1A register to 0
  TCCR1B = 0; // set TCCR1B register to 0
  TCNT1  = 0; // set counter value to 0
  
  OCR1A = 25; // set compare match register
  
  TCCR1B |= (1 << CS12); // Set CS12 and CS10 bits for 1:256 prescaler
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei(); // allow interrupts
}
