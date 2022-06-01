#include <QTRSensors.h>
QTRSensors qtr;
const int Buzzer = 3;
int TempsPrecedent = 0;
int TempsActuel = 0;
bool FermeAvant = false;
bool Alarme = false;
const int Button = 2;
int Diff = 0;
const uint8_t SensorCount = 1;
uint16_t sensorValues[SensorCount];
void setup()
{
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]) {
    A0
  }, SensorCount);
  pinMode(Buzzer, OUTPUT);
  pinMode(Button, INPUT);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  Serial.print(qtr.calibrationOn.minimum[0]);
  Serial.print(' ');
  Serial.println();
  Serial.print(qtr.calibrationOn.maximum[0]);
  Serial.print(' ');
  Serial.println();
  Serial.println();
  delay(1000);
  TempsPrecedent = millis();
}

void loop()
{
  uint16_t position = qtr.readLineBlack(sensorValues);
  TempsActuel = millis();
  if (sensorValues[0] < 100)
  {
    //Serial.print("détection fermé");
    if (FermeAvant == true)
    {
      //Serial.println(TempsPrecedent);
      Diff = TempsActuel - TempsPrecedent;
      if (Diff > 500)
      {
        Alarme = true;
      }
    }
    else if (FermeAvant == false)
    {
      TempsPrecedent = TempsActuel;
      FermeAvant = true;
    }
  }
  else if (sensorValues[0] > 100)
  {
    if (FermeAvant == true)
    {
      TempsPrecedent = TempsActuel;
      FermeAvant = false;
    }
    else if (FermeAvant == false)
    {
      Diff = TempsActuel - TempsPrecedent;
      if (Diff > 10000)
      {
        Alarme = true;
      }
    }
  }
  if (Alarme == true)
  {
    for (int t = 0; t < 200; t++)
    {
      digitalWrite(Buzzer, HIGH);
      delay(1);
      digitalWrite(Buzzer, LOW);
      delay(1);
    }
  }
  /*while (Alarme == true)
    {
    if (Button == HIGH)
    {
      Alarme = false;
      delay(5000);
      TempsPrecedent = millis();
    }
    else
    {
      digitalWrite(Buzzer, HIGH);
      delay(5);
      digitalWrite(Buzzer, LOW);
      delay(5);
    }
    }*/
  Serial.println(sensorValues[0]);
  Serial.println(' ');
}
