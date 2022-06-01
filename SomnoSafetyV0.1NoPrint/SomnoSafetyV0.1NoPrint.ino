#include <QTRSensors.h>
QTRSensors qtr;
const int BuzzerPin = 3;
int TempsPrecedent = 0;
int TempsActuel = 0;
int TempsCalib = 0;
int Temps2H = 0;
bool FermeAvant = false;
bool Alarme = false;
bool Alerte2H = false;
const int ButtonPin = 2;
int Diff = 0;
int DiffCalib = 0;
int Diff2H = 0;
int Button = 0;
const uint8_t SensorCount = 1;
uint16_t sensorValues[SensorCount];
void setup()
{
  Serial.begin(9600);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]) {
    A0
  }, SensorCount);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(Button, INPUT);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (int t = 0; t < 50; t++)
  {
    digitalWrite(BuzzerPin, HIGH);
    delay(1);
    digitalWrite(BuzzerPin, LOW);
    delay(1);
  }
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  for (int t = 0; t < 100; t++)
  {
    digitalWrite(BuzzerPin, HIGH);
    delay(1);
    digitalWrite(BuzzerPin, LOW);
    delay(1);
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  TempsPrecedent = millis();
  TempsCalib = millis();
  Temps2H = millis();
}

void loop()
{
  Button = digitalRead(ButtonPin);
  uint16_t position = qtr.readLineBlack(sensorValues);
  TempsActuel = millis();
  // Alerte régulière
  Diff2H = TempsActuel - Temps2H;
  if (Alerte2H == false)
  {
    if (Diff2H > 7200000)
    {
      for (int t = 0; t < 5; t++)
      {
        for (int t = 0; t < 20; t++)
        {
          digitalWrite(BuzzerPin, HIGH);
          delay(1);
          digitalWrite(BuzzerPin, LOW);
          delay(1);
        }
        delay(50);
        for (int t = 0; t < 20; t++)
        {
          digitalWrite(BuzzerPin, HIGH);
          delay(1);
          digitalWrite(BuzzerPin, LOW);
          delay(1);
        }
      }
      Alerte2H = true;
    }
    else
    {
      if (Diff2H > 3600000)
      {
        for (int t = 0; t < 5; t++)
        {
          for (int t = 0; t < 20; t++)
          {
            digitalWrite(BuzzerPin, HIGH);
            delay(1);
            digitalWrite(BuzzerPin, LOW);
            delay(1);
          }
          delay(50);
          for (int t = 0; t < 20; t++)
          {
            digitalWrite(BuzzerPin, HIGH);
            delay(1);
            digitalWrite(BuzzerPin, LOW);
            delay(1);
          }
        }
      }
      Temps2H = millis();
    }
    // calibration toutes les 20 minutes
    DiffCalib = TempsActuel - TempsCalib;
    if (DiffCalib > 1198000)
    {
      for (int t = 0; t < 50; t++)
      {
        digitalWrite(BuzzerPin, HIGH);
        delay(1);
        digitalWrite(BuzzerPin, LOW);
        delay(1);
      }
      for (uint16_t i = 0; i < 400; i++)
      {
        qtr.calibrate();
      }
      for (int t = 0; t < 100; t++)
      {
        digitalWrite(BuzzerPin, HIGH);
        delay(1);
        digitalWrite(BuzzerPin, LOW);
        delay(1);
      }
      TempsCalib = millis();
    }
    // état de l'oeil
    if (sensorValues[0] < 100)
    {
      if (FermeAvant == true)
      {
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
    // alerte
    while (Alarme == true)
    {
      if (Button == 0)
      {
        Alarme = false;
        TempsPrecedent = millis();
      }
      else
      {
        digitalWrite(BuzzerPin, HIGH);
        delay(5);
        digitalWrite(BuzzerPin, LOW);
        delay(5);
      }
    }
  }
