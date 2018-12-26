//#define potentPin 0
//Библиотеки
#include <MovingAverage.h>
#include <RBD_Timer.h>
// Переменные
//Переменные пинов

//Аналоговые входа
int compressorPinA = 0;                                        //Назначение номера Аналогового входа датчика давления в компрессоре
int levelSensorFL = 1;                                         //Назначение номера Аналогового входа датчика уровня переднего левого колеса
int levelSensorFR = 2;                                         //Назначение номера Аналогового входа датчика переднего правого колеса
int levelSensorRL = 3;                                         //Назначение номера Аналогового входа датчика заднего левого колеса
int levelSensorRR = 4;                                         //Назначение номера Аналогового входа датчика заднего правого колеса

//Дискретные входа

//Дискретные выхода
int pinDCompressor = 23;                                       //Дискретный выход управления насосом
int pinValFill = 25;                                          //Дискретный выход управления клапаном общего набора давления
int pinValEmpt = 27;                                          //Дискретный выход управления клапаном общего сброса давления
int pinValFL = 29;                                            //Дискретный выход управления клапаном переднего левого колеса
int pinValFR = 31;                                            //Дискретный выход управления клапаном переднего правого колеса
int pinValRL = 33;                                            //Дискретный выход управления клапаном заднего левого колеса
int pinValRR = 35;                                            //Дискретный выход управления клапаном заднего правого колеса
//Временные переменные функции compresorControlFunc()
float pressLevelInt;
float pressLevel;
bool compressor;
//Уставки
//Общие уставки

//Уставки функции compresorControlFunc()
int pressureHighLevel = 10;                                  //Уставка верхнего давления при котором компрессор будет отключаться                        
int pressureLowLevel = 8;                                    //Уставка нижнего давления при котором компрессор будет включаться
int timeSetValveOn = 1000;                                   // уставка времени работы клапана
int timeSet = 3000;                                          //уставка времени ожидания реакция системы

//Уставки функции levelControlFunc()
int levelSet = 250;                                          //переменная уставки положения уровня
int levelDelta = 50;                                         //Дельта при которой регулятор не реагирует


//ExponentialFilter<long> ADCFilter(5, 0);
RBD::Timer timerValveOn;
RBD::Timer timer;

// Create moving average with alpha = 0.1f.
MovingAverage average(0.05f);
void setup() {
  Serial.begin(9600);

}

void loop() {
  //#7скореевсего будут условия работы подвески{
  levelControlFuncFL(timeSet, timeSetValveOn, levelSensorFL,  levelDelta, levelSet);  //регулирования левого переднего балона
  levelControlFuncFR(timeSet, timeSetValveOn, levelSensorFR,  levelDelta, levelSet);  //#3 вставить фунцкию levelControlFuncFR(); регулирования правого переднего балона
  //#4вставить фунцкию levelControlFuncRL(); регулирования левого заднего балона
  //#5вставить фунцкию levelControlFuncRR(); регулирования правого заднего балона
  //}
  compresorControlFunc(compressorPinA, pinDCompressor, pressureHighLevel, pressureLowLevel);

}
//#1откатать тело функции регулятора уровня
void levelControlFuncFL(int timeSetF, int timerValveOnF, int levelSensorFLF, int levelDeltaF, int levelSetF) {
   timer.setTimeout(timeSetF);
   int levelIn = average.update( analogRead(levelSensorFLF) );
   int levelInMod = map(levelIn, 0, 1023, 0, 500);
 if (timer.onRestart()) {
    if ( levelSetF - levelDeltaF > levelInMod ) {     //Условие включения накачки
        digitalWrite(pinValFill, HIGH);
        digitalWrite(pinValFL, HIGH);
        timerValveOn.setTimeout(timerValveOnF);
       //test = 50;
     }
    
    if ( levelSetF + levelDeltaF < levelInMod ) {
        digitalWrite(pinValEmpt, HIGH);
        digitalWrite(pinValFL, HIGH);
     }
   }
  if(timerValveOn.onRestart()) {
      digitalWrite(pinValFill, LOW);
      digitalWrite(pinValEmpt, LOW);
      digitalWrite(pinValFL, LOW);
    }
  }
 
void  levelControlFuncFR(int timeSetF, int timerValveOnF, int levelSensorFRF,  int levelDeltaF, int levelSetF) {
  timer.setTimeout(timeSetF);
  int levelIn = average.update( analogRead(levelSensorFRF) );
  int levelInMod = map(levelIn, 0, 1023, 0, 500);
  if (timer.onRestart()) {
    if ( levelSetF - levelDeltaF > levelInMod ) {     //Условие включения накачки
        digitalWrite(pinValFill, HIGH);
        digitalWrite(pinValFR, HIGH);
        timerValveOn.setTimeout(timerValveOnF);
      //test = 50;
     }
    
    if ( levelSetF + levelDeltaF < levelInMod ) {
        digitalWrite(pinValEmpt, HIGH);
        digitalWrite(pinValFL, HIGH);
     }
   }

   if(timerValveOn.onRestart()) {
      digitalWrite(pinValFill, LOW);
      digitalWrite(pinValEmpt, LOW);
      digitalWrite(pinValFL, LOW);
    }
  }
  
  
 /* 
 * void levelControlFuncRL(){
 * 
 * }
 * 
 *void levelControlFuncRR(){
 *
 *}
 * 
 * 
 * 
 */
void compresorControlFunc(int compressorPinAF, int pinDCompressorF, int pressureHighLevelF, int pressureLowLevelF) {

  int valAnalog0;                                            //Промежуточная переменная хронящая значение давления(необработанная)
  valAnalog0 = analogRead(compressorPinAF);
  pressLevelInt = map(valAnalog0, 0, 1023, 0, 150);         //Функцией map задаю пределы величины давления простая пропорция
  pressLevel = pressLevelInt / 10;                          // деление на  10 заведомо в десять раз увеличинного диапазона дает десятичные дроби
  if (pressLevel < pressureLowLevelF) {
    compressor = true;
    digitalWrite(pinDCompressorF, HIGH);
  }


  if (pressLevel > pressureHighLevelF) {
    compressor = false;
    digitalWrite(pinDCompressorF, LOW);
  }
  Serial.print("In: ");
  Serial.print(pressLevel);
  Serial.print(" - Out: ");
  Serial.println(compressor);

  //  constrain(valAnalog0, 0, 500);
  //Serial.println(v);
  delay(200);  //!!!!!!!!!!!!!!!!!!!!Убрать впосле отладки
}
