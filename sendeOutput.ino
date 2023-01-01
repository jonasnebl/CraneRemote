void sendeOutput()
{
  for (int i = 0; i <= 5; i++)
  {
    byte c1 = data[i] >> 8;
    byte c2 = data[i] - c1 * 256;
    Serial.write(c1);
    Serial.write(c2);
  }
  Serial.write(VRx);
  Serial.write(VRy);
  Serial.write(button);
    
  Serial.println();
}
