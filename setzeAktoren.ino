void setzeAktoren()
{
    int KatzeSpeed = map(SpeedInput[0],0,255, -maxSpeedKatze, maxSpeedKatze);
    if (abs(KatzeSpeed) <= 50)
    {
      KatzeSpeed = 0;
    }
    
    int HakenSpeed = map(SpeedInput[1],0,255, -maxSpeedHaken, maxSpeedHaken);
    if (abs(HakenSpeed) <= 20)
    {
      HakenSpeed = 0;
    }
    
    int DrehkranzSpeed = (SpeedInput[2] - 127) * maxSpeedDrehkranz / 128;
  
    Laufkatze.setSpeed(KatzeSpeed);
    Haken.setSpeed(HakenSpeed);
    Drehkranz.setSpeed(DrehkranzSpeed);
}
