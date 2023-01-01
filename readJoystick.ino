void readJoystick()
{
  VRx = analogRead(A3) / 4;
  VRy = analogRead(A2) / 4;
  button = digitalRead(10);
}
