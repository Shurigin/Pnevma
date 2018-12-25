//#define potentPin 0
// Переменные
//Переменные пинов

//Аналоговые входа
int compressorPinA = 0;                                        //Аналоговый вход датчика давления в компрессоре
int levelSensorFL = 1;                                         //Аналоговый вход датчика уровня переднего левого колеса
int levelSensorFR = 2;                                         //Аналоговый вход датчика уровня переднего правого колеса
int levelSensorRL = 3;                                         //Аналоговый вход датчика уровня заднего левого колеса
int levelSensorFR = 4;                                         //Аналоговый вход датчика уровня заднего правого колеса

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

//ExponentialFilter<long> ADCFilter(5, 0);
void setup() {
  Serial.begin(9600);

}

void loop() {
  compresorControlFunc(compressorPinA, pinDCompressor, pressureHighLevel, pressureLowLevel);

}
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
