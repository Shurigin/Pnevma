
#include <MovingAverage.h>
#include <RBD_Timer.h>

int levelSensorFL = 1;
int levelSet = 250;    //переменная уставки положения уровня
int levelDelta = 50;   //Дельта при которой регулятор не реагирует
//Дискретные выходы
int pinValFill = 25;
int pinValEmpt = 27;
int pinValFL = 29;
int timeSetValveOn = 1000; // уставка времени работы клапана
int timeSet = 3000;
int test ;
RBD::Timer timerValveOn;
RBD::Timer timer;

// Create moving average with alpha = 0.1f.
MovingAverage average(0.05f);

void setup() {
  // Begin serial.
  Serial.begin(9600);
  // Initialize the average with the first value from analogRead().

  average.reset( analogRead(levelSensorFL) );
}

void loop() {
  timer.setTimeout(timeSet);
  // Update the value moving average with new analog reading and print the
  // moving average's current value.
  int levelIn = average.update( analogRead(levelSensorFL) );
  int levelInMod = map(levelIn, 0, 1023, 0, 500);
  if (timer.onRestart()) {
    if ( levelSet - levelDelta > levelInMod ) {     //Условие включения накачки
      digitalWrite(pinValFill, HIGH);
      digitalWrite(pinValFL, HIGH);
      timerValveOn.setTimeout(timeSetValveOn);
      test = 50;
    }
    //допоисать запуск таймера и выключение накачки
    if (  levelSet + levelDelta < levelInMod ) {
      digitalWrite(pinValEmpt, HIGH);
      digitalWrite(pinValFL, HIGH);
    }
  }
  if(timerValveOn.onRestart()) {
    digitalWrite(pinValFill, LOW);
    digitalWrite(pinValEmpt, LOW);
    digitalWrite(pinValFL, LOW);
  }
  
  //допоисать запуск таймера и выключение накачки

  Serial.print("In: ");
  Serial.print(analogRead(levelSensorFL));
  Serial.print(" - Out: ");
  //Serial.print( levelIn );
  Serial.print( test );
  Serial.print(" - OutMod: ");

  Serial.println( levelInMod );
  delay(100);
}
