ISR(TIMER1_COMPA_vect) 
{ // function which will be called when an interrupt occurs at timer 1
  Laufkatze.runSpeed();
  Haken.runSpeed();
  Drehkranz.runSpeed();
}
