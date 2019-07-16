#include <DFPlayer_Mini_Mp3.h>

#include <SoftwareSerial.h>

#include <SPI.h>

#include <TM1637.h>

#include <MFRC522.h>

#include <Keypad.h>

#include "FastLED.h"          // библиотека для работы с лентой

/*коды на панели
1)238367
2)038195
3)883615
4)114141
5)983583
6)373983
7)384991
8)862815
9)569247
10)590879
 */

 /* ключи RFID
1)2188238367
2)1140038195
3)2190883615
4)2741114141
5)2192983583
6)2193373983
7)2185384991
8)2187862815
9)2194569247
10)2184590879 
  */

/*ключи админа
1)1143764744
2)1156837384
3)1146457864

 */

//========================RFID===================
MFRC522 rfid(53, 5); //піни SDA, RST
unsigned long Key, KeyTemp; // для зберікання ключа
//==============================================
//===============ЛЕНТА========================
#define LED_COUNT 30         // число сигментов в  ленте 
#define LED_CEIRCLE 3         // число сигментов в однов кольце
#define LED_DT0 11            // пин, куда подключен DIN ленты
int max_bright = 255;         // максимальная яркость (0 - 255)
struct CRGB leds0[LED_COUNT];

int brightness = 255;
int thisdelay = 300;          //-FX LOOPS DELAY VAR
int thisstep = 0;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int id = 0;
int id1 = 0;
int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
//=====================================================
//====================СВЕТОДИОДЫ=====================
int led1_1 = 38;
int led1_2 = 36;
int led1_3 = 34;
int led1_4 = 32;
int led1_5 = 30;
int led1_6 = 28;
int led2_1 = 26;
int led2_2 = 24;
int led2_3 = 23;
int led2_4 = 25;
int led2_5 = 27;
int led2_6 = 29;
int led3_1 = 31;
int led3_2 = 33;
int led3_3 = 35;
int led3_4 = 37;
int led3_5 = 41;
int led3_6 = 43;
int led1 = 45;
int led2 = 47;

//===================ВХЛДЫ ОТ ПРОВОДОВ===========
int input1 = A8;
int input2 = A9;
int input3 = A10;
int input4 = A11;
int input5 = A12;
int input6 = A13;
//==============================================
//=====================КЛАВИАТУРА ПАРОЛЯ==============
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A7, A6, A8, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A3, A2, A1, A0}; //connect to the column pinouts of the keypad


Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//==============================================
//================ШАГОВЫЙ ДВИГАТЕЛЬ============
int dirOut = 13;
int stepOut = 12;
//============================================
String string_code1 = "1412025000";
String string_code2 = "2210221000";
String string_code3 = "1182102018";
String string_code4 = "1216182000";
String string_code5 = "2210152000";
String string_code6 = "1861216185";
String string_code7 = "2210151026";
String string_code8 = "1965131600";
String string_code9 = "1816561600";
String string_code10 = "4161328220";

String string_code = "25062009";

String string;


int timer_start = 3; //D3
//int timer_stop = 2; //D2
int timer_on = 49;

int delay_number = 300;
int delay_between_number = 1000;

unsigned long time_code = 0;

unsigned long time_start = 0;
unsigned long time_final = 0;

int b = 0;          //счетчик на количество шагов для шагового двигателя

int flag_led = 0;       //флаг для лєнти

int flag = 0;         //флаг последовательности
int flag_mp3_code = 0;
int flag_start = 0;
int flag_final = 0;

int flag_number1 = 0;       //флаги для отображения подсказок 
int flag_number2 = 0;
int flag_number3 = 0;
int flag_number4 = 0;
int flag_number5 = 0;
int flag_number6 = 0;
int flag_number7 = 0;
int flag_number8 = 0;
int flag_number9 = 0;
int flag_number10 = 0;

int flag_admin1=0;
int flag_admin2=0;
int flag_admin3=0;

int flag_keypad1 = 0;
int flag_keypad2 = 0;
int flag_keypad3 = 0;
int flag_keypad4 = 0;
int flag_keypad5 = 0;
int flag_keypad6 = 0;

int game=0;

SoftwareSerial mySerial(2, 3);
void setup(){
  Serial.begin(9600);
  Serial3.begin(9600);
  mySerial.begin(9600);

  SPI.begin();   // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 card

  mp3_set_serial (Serial3);  //   софтовый серийный порт для мп3
  delay (100);                              //  обязательная задержка между командами
  mp3_set_volume (20);
  mp3_stop();

  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость
  LEDS.addLeds<WS2811, LED_DT0, GRB>(leds0, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);          // погасить все светодиоды
  LEDS.show();                     // отослать команду
  
  pinMode(led1_1,OUTPUT);
  pinMode(led1_2,OUTPUT);
  pinMode(led1_3,OUTPUT);
  pinMode(led1_4,OUTPUT);
  pinMode(led1_5,OUTPUT);
  pinMode(led1_6,OUTPUT);
  pinMode(led2_1,OUTPUT);
  pinMode(led2_2,OUTPUT);
  pinMode(led2_3,OUTPUT);
  pinMode(led2_4,OUTPUT);
  pinMode(led2_5,OUTPUT);
  pinMode(led2_6,OUTPUT);
  pinMode(led3_1,OUTPUT);
  pinMode(led3_2,OUTPUT);
  pinMode(led3_3,OUTPUT);
  pinMode(led3_4,OUTPUT);
  pinMode(led3_5,OUTPUT);
  pinMode(led3_6,OUTPUT);
  digitalWrite(led1_1,LOW);
  digitalWrite(led1_2,LOW);
  digitalWrite(led1_3,LOW);
  digitalWrite(led1_4,LOW);
  digitalWrite(led1_5,LOW);
  digitalWrite(led1_6,LOW);
  digitalWrite(led2_1,LOW);
  digitalWrite(led2_2,LOW);
  digitalWrite(led2_3,LOW);
  digitalWrite(led2_4,LOW);
  digitalWrite(led2_5,LOW);
  digitalWrite(led2_6,LOW);
  digitalWrite(led3_1,LOW);
  digitalWrite(led3_2,LOW);
  digitalWrite(led3_3,LOW);
  digitalWrite(led3_4,LOW);
  digitalWrite(led3_5,LOW);
  digitalWrite(led3_6,LOW);
  
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
  pinMode(7, INPUT);
  //pinMode(input1,INPUT_PULLUP);
  //pinMode(input2,INPUT_PULLUP);
 // pinMode(input3,INPUT_PULLUP);
  //pinMode(input4,INPUT_PULLUP);
  //pinMode(input5,INPUT_PULLUP);
  //pinMode(input6,INPUT_PULLUP);

  pinMode(timer_on, OUTPUT);
  pinMode(timer_start, OUTPUT);
  //pinMode(timer_stop, OUTPUT);
  //digitalWrite(timer_start, HIGH);

  //digitalWrite(timer_stop, LOW); //Обнуляємо таймер
  //delay(2000);
  //digitalWrite(timer_stop, HIGH);

  pinMode(dirOut,OUTPUT);
  pinMode(stepOut,OUTPUT);

  one_color_all(255, 255, 255);
  LEDS.show();

  //lapm_test();
  //number_test();
}
  
void loop()
{ 
  admin();
  for (byte i = 0; i < 4; i++) { //Обнуляєм попередній ключ
      rfid.uid.uidByte[i] = 0;
    }
    if ( ! rfid.PICC_IsNewCardPresent()); // пошук нового ключа
    if ( ! rfid.PICC_ReadCardSerial()); // вибір ключа
    for (byte i = 0; i < 4; i++) {
      KeyTemp = rfid.uid.uidByte[i];
      Key = Key*256 + KeyTemp;
    }
    
    if(Key != 0)
    {
      Serial.println(Key);
    }

    if(digitalRead(9)==1) Key = 1143764744; //Перехват с пульта "А" Старт
    if(Key == 1143764744 && flag_admin1==0)
    {
      flag_start=1;
      flag_admin1=1;
      one_color_all(255, 0, 0);
      LEDS.show();
      digitalWrite(timer_on, HIGH); //запустить таймер
        delay(2000);
        digitalWrite(timer_start, LOW);
        delay(500);
        digitalWrite(timer_start, HIGH);
         Serial.println("timer_start");
        mp3_set_serial(Serial3);
        mp3_set_volume(20);
        mp3_play(1);}
          

 if(Key == 2188238367 && flag_number1==0 && flag_start==1){flag_number1 = 1; callstep(string_code1);}// 1
    if(Key == 1140038195 && flag_number2==0 && flag_start==1){flag_number2 = 1; callstep(string_code2);}// 2
    if(Key == 2190883615 && flag_number3==0 && flag_start==1){flag_number3 = 1; callstep(string_code3);}// 3
    if(Key == 2741114141 && flag_number4==0 && flag_start==1){flag_number4 = 1; callstep(string_code4);}// 4
    if(Key == 2192983583 && flag_number5==0 && flag_start==1){flag_number5 = 1; callstep(string_code5);}// 5
    if(Key == 2193373983 && flag_number6==0 && flag_start==1){flag_number6 = 1; callstep(string_code6);}// 6
    if(Key == 2185384991 && flag_number7==0 && flag_start==1){flag_number7 = 1; callstep(string_code7);}// 7
    if(Key == 2187862815 && flag_number8==0 && flag_start==1){flag_number8 = 1; callstep(string_code8);}// 8
    if(Key == 2194569247 && flag_number9==0 && flag_start==1){flag_number9 = 1; callstep(string_code9);}// 9
    if(Key == 2184590879 && flag_number10==0 && flag_start==1){flag_number10 = 1; callstep(string_code10);}// 10
    if(digitalRead(8)==1){flag=1;}

    if(flag==1)// step up
    {  
      b=0;
       mp3_play(8);
           //Крутити двигун на 500 кроків
      while(b<610) {
        b++;
        digitalWrite(dirOut, HIGH);
  
        digitalWrite(stepOut, LOW);
  
        delay(9);
  
        digitalWrite(stepOut, HIGH);
      }
      mp3_stop();
      delay(100);
      mp3_play(1);
      flag=0;
      game++;
    }

 if(game==9) {
  flag_led =10;digitalWrite(timer_on, LOW);time_final=millis(); final(); flag_admin2 = 1;
  }
 
 
}

void callstep(String thispass)
{
  digitalWrite(led1_1,HIGH);
  digitalWrite(led1_2,HIGH);
  digitalWrite(led1_3,HIGH);
  digitalWrite(led1_4,HIGH);
  digitalWrite(led1_5,HIGH);
  digitalWrite(led1_6,HIGH);
  digitalWrite(led2_1,HIGH);
  digitalWrite(led2_2,HIGH);
  digitalWrite(led2_3,HIGH);
  digitalWrite(led2_4,HIGH);
  digitalWrite(led2_5,HIGH);
  digitalWrite(led2_6,HIGH);
  digitalWrite(led3_1,HIGH);
  digitalWrite(led3_2,HIGH);
  digitalWrite(led3_3,HIGH);
  digitalWrite(led3_4,HIGH);
  digitalWrite(led3_5,HIGH);
  digitalWrite(led3_6,HIGH);
 m1:  char pressed=customKeypad.getKey();
      if(pressed){
            Serial.println(pressed);
            string += pressed;
            Serial.println(string);
            if(string.length()==10)
            {
              if(string.equals(thispass)){
              flag=1;  
                digitalWrite(led1_1,LOW);
  digitalWrite(led1_2,LOW);
  digitalWrite(led1_3,LOW);
  digitalWrite(led1_4,LOW);
  digitalWrite(led1_5,LOW);
  digitalWrite(led1_6,LOW);
  digitalWrite(led2_1,LOW);
  digitalWrite(led2_2,LOW);
  digitalWrite(led2_3,LOW);
  digitalWrite(led2_4,LOW);
  digitalWrite(led2_5,LOW);
  digitalWrite(led2_6,LOW);
  digitalWrite(led3_1,LOW);
  digitalWrite(led3_2,LOW);
  digitalWrite(led3_3,LOW);
  digitalWrite(led3_4,LOW);
  digitalWrite(led3_5,LOW);
  digitalWrite(led3_6,LOW);
              }else //неправильний  
                 {
                  string=""; 
                  Serial.println("wrong");
                  one_color_all(0, 0, 0);
                  LEDS.show();
                  delay(200);
                  one_color_all(255, 0, 0);
                  LEDS.show();
                       one_color_all(0, 0, 0);
                  LEDS.show();
                  delay(200);
                  one_color_all(255, 0, 0);
                  LEDS.show();
                       one_color_all(0, 0, 0);
                  LEDS.show();
                  delay(200);
                  one_color_all(255, 0, 0);
                  LEDS.show();
                 }  
              }
      }
      if(flag==0)  goto m1;
}

void admin(){
   flag_admin1=0;
    flag_admin2=0;
    for (byte i = 0; i < 4; i++) { //Обнуляєм попередній ключ
      rfid.uid.uidByte[i] = 0;
    }
    if ( ! rfid.PICC_IsNewCardPresent()); // пошук нового ключа
    if ( ! rfid.PICC_ReadCardSerial()); // вибір ключа
    for (byte i = 0; i < 4; i++) {
      KeyTemp = rfid.uid.uidByte[i];
      Key = Key*256 + KeyTemp;
    }
    if(digitalRead(10)==1) Key = 1156837384; //Перехват с пульта "В" Финал
    if(Key == 1156837384 && flag_admin2==0){flag_led =10;digitalWrite(timer_on, LOW);time_final=millis(); final(); flag_admin2 = 1;}// 1

    
    
}
void final(){
  mp3_set_serial(Serial3);
  mp3_set_volume(30);
  mp3_play(10);
  digitalWrite(timer_on, LOW);
  while(millis()-time_final<10000){
    id = id + 1;
    if (id == 3  ) {
      id = 0;
    } 
      for (int i = 0; i < 3; i++ ) {
        if (i == id) {
          leds0[i].setRGB( 0, 0, 255);
          leds0[i+3].setRGB( 0, 0, 255);
          leds0[i+6].setRGB( 0, 0, 255);
          leds0[i+9].setRGB( 0, 0, 255);
          leds0[i+12].setRGB( 0, 0, 255);
          leds0[i+15].setRGB( 0, 0, 255);
          leds0[i+18].setRGB( 0, 0, 255);
          leds0[i+21].setRGB( 0, 0, 255);
          leds0[i+24].setRGB( 0, 0, 255);
          leds0[i+27].setRGB( 0, 0, 255);
          
        }
        else {
          leds0[i] = CHSV(0, 0, 0);
          leds0[i+3] = CHSV(0, 0, 0);
          leds0[i+6] = CHSV(0, 0, 0);
          leds0[i+9] = CHSV(0, 0, 0);
          leds0[i+12] = CHSV(0, 0, 0);
          leds0[i+15] = CHSV(0, 0, 0);
          leds0[i+18] = CHSV(0, 0, 0);
          leds0[i+21] = CHSV(0, 0, 0);
          leds0[i+24] = CHSV(0, 0, 0);
          leds0[i+27] = CHSV(0, 0, 0);
        }
      }
    LEDS.show();
    delay(thisdelay);
    b=0;
  }
  one_color_green(0,30);
  LEDS.show();
  while(b<6200) {
        b++;
        digitalWrite(dirOut, LOW);
  
        digitalWrite(stepOut, LOW);
  
        delay(1);
  
        digitalWrite(stepOut, HIGH);
 }
 one_color_all(0, 0, 0);
 LEDS.show();
 flag=5;
  
}


void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds0[i].setRGB( cred, cgrn, cblu);
  }
}
void one_color_green(int pin_start, int pin_stop ){
  for (int i = pin_start ; i < pin_stop; i++ ) {
    leds0[i].setRGB( 0, 0, 255);
  }
}




