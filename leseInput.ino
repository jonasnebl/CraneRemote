void leseInput()
{
  for (int i = 0; i <= 2; i++)
  {
    if (Serial.available() > 0)
    {
      SpeedInput[i] = Serial.read();
    }
  }
}
