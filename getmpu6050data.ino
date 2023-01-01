void getmpu6050data() 
{
  //Read the data if available in buffer
  if (radio.available())
  {
    int dataTemp[6];
    radio.read(&dataTemp, sizeof(dataTemp));

    for(int i = 0; i <= 5 ; i++)
    {
      data[i] = dataTemp[i];
    }
  }
}
