#include "TM1637.h" // подключение библиотеки
#define CLK 7
#define DIO 8
int timer_val = 30; // Обратный отсчет в минутах
int timer_seconds = 00; // Отображение секунд
// Переменные для хранения индивидуальных номеров
int firstnum = 0;
int secondnum = 0;
int thirdnum = 0;
int fournum = 0;
unsigned long time0 = 0;
TM1637 tm1637(CLK, DIO); // Настройка индикатора
//====================================================
int flag = 0;
int in5 = 5;
int in6 = 6;

void setup() {
  Serial.begin(9600);
  
  pinMode(in6, INPUT_PULLUP);
  pinMode(in5, INPUT_PULLUP);
}

void loop() 
{
    Serial.println("in5  in6");
    Serial.print(digitalRead(in5));
    Serial.print("  ");
    Serial.println(digitalRead(in6));
    
    tm1637.set(BRIGHT_TYPICAL); // Уровень яркости
    // BRIGHT_DARKEST = 0,BRIGHTEST = 7 BRIGHT_TYPICAL = 2&#894;
    tm1637.point(POINT_ON); // Активация точки посредине
    if (timer_val > 9) {
      firstnum = timer_val / 10 % 10;
      secondnum = timer_val % 10;
    }
    else {
      secondnum = timer_val;
    }
    if (timer_seconds > 9) {
      thirdnum = timer_seconds / 10 % 10;
      fournum = timer_seconds % 10;
    }
    else {
      thirdnum = 0;
      fournum = timer_seconds;
    }
    // Отображаем обратный отсчет
    tm1637.clearDisplay(); // Очистка дисплея
    if (timer_val > 9) {
      tm1637.display(0, firstnum);
    }
    if (timer_val > 0) {
      tm1637.display(1, secondnum);
    }
    if (timer_seconds > 9 || timer_val > 0) {
      tm1637.display(2, thirdnum);
    }
    tm1637.display(3, fournum);
    // Decrease seconds
    timer_seconds = timer_seconds - 1;
    
    delay(1000); // Delay of 1 second
    // Decrease timer
    if (timer_seconds == -1) {
      timer_val = timer_val - 1;
      timer_seconds = 59;
    }
}
