/* Fernsteuerung des Krans über die serielle Schnittstelle
 * Der Arduino führt keine eigene Steuerungslogik aus.
 */
#include <AccelStepper.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN
//address through which two modules communicate
const byte address[6] = "00001";

//Erzeugen der Motorobjekte;
const int dirPinKatze =  4, stepPinKatze =  5;
const int dirPinHaken = 6, stepPinHaken = 7;
const int stepPinDrehkranz = 3, dirPinDrehkranz = 2;
const int motorInterfaceType = 1;
AccelStepper Laufkatze = AccelStepper(motorInterfaceType, stepPinKatze, dirPinKatze);
AccelStepper Haken = AccelStepper(motorInterfaceType, stepPinHaken, dirPinHaken);
AccelStepper Drehkranz = AccelStepper(motorInterfaceType, stepPinDrehkranz, dirPinDrehkranz);

//Höchstgeschwindigkeiten
const int maxSpeedKatze = 1500, maxSpeedHaken = 1000, maxSpeedDrehkranz = 120;

byte SpeedInput[] = {127,127,127};
int data[] = {0,0,0,0,0,0};
byte VRx, VRy, button;

void setup() {
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(10, INPUT);
  
  Serial.begin(115200);

  //Starten der Funkverbindung zum Sensor
  radio.begin();
  radio.openReadingPipe(0, address); //Setzen der Addresse
  radio.startListening(); //Modul als Empfänger deklarieren

  //Setzen der Motorhöchstgeschwindigkeiten
  Laufkatze.setMaxSpeed(maxSpeedKatze);
  Haken.setMaxSpeed(maxSpeedHaken);
  Drehkranz.setMaxSpeed(maxSpeedDrehkranz);

  enableInterrupts();
}

void loop() 
{
  leseInput();
  setzeAktoren();

  readJoystick();
  getmpu6050data();
  sendeOutput(); 
  delay(20);
}
