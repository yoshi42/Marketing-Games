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
byte rowPins[ROWS] = {A7, A6, A5, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A3, A2, A1, A0}; //connect to the column pinouts of the keypad


Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//==============================================
//=====================КЛАВИАТУРА ДЛЯ ПРОВОДОВ==============
const byte ROWS1 = 1; //four rows
const byte COLS1 = 6; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys1[ROWS1][COLS1] = {
  {'1','2','3','4','5','6'}
};
byte rowPins1[ROWS1] = {A8}; //connect to the row pinouts of the keypad
byte colPins1[COLS1] = {A9, A10, A11, A12, A13, A14}; //connect to the column pinouts of the keypad


Keypad customKeypad1 = Keypad( makeKeymap(hexaKeys1), rowPins1, colPins1, ROWS1, COLS1);
//==============================================
//================ШАГОВЫЙ ДВИГАТЕЛЬ============
int dirOut = 13;
int stepOut = 12;
//============================================
String string_code1 = "238367";
String string_code2 = "038195";
String string_code3 = "883615";
String string_code4 = "114141";
String string_code5 = "983583";
String string_code6 = "373983";
String string_code7 = "384991";
String string_code8 = "862815";
String string_code9 = "569247";
String string_code10 = "590879";

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
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  
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

  //lapm_test();
  //number_test();
}
  
void loop(){ 
    
 
  admin();
  color_bounce();
  //=====================1 ЕТАП(Зчитати всі 10 карточок)=====================
  if(flag == 0)
  {
    if(flag_led==10)
    {
      flag_led=11;
      time_final=millis();
    }

    if(flag_led==11)
    {
      final();
    }
    
    flag_start=0;
    for (byte i = 0; i < 4; i++) { //Обнуляєм попередній ключ
      rfid.uid.uidByte[i] = 0;
    }
    if ( ! rfid.PICC_IsNewCardPresent()); // пошук нового ключа
    if ( ! rfid.PICC_ReadCardSerial()); // вибір ключа
    for (byte i = 0; i < 4; i++) {
      KeyTemp = rfid.uid.uidByte[i];
      Key = Key*256 + KeyTemp;
    }
    if(Key != 0){
      Serial.println(Key);
    }

    if(Key == 2188238367 && flag_number1==0){flag_number1 = 1;}// 1
    if(Key == 1140038195 && flag_number2==0){flag_number2 = 1;}// 2
    if(Key == 2190883615 && flag_number3==0){flag_number3 = 1;}// 3
    if(Key == 2741114141 && flag_number4==0){flag_number4 = 1;}// 4
    if(Key == 2192983583 && flag_number5==0){flag_number5 = 1;}// 5
    if(Key == 2193373983 && flag_number6==0){flag_number6 = 1;}// 6
    if(Key == 2185384991 && flag_number7==0){flag_number7 = 1;}// 7
    if(Key == 2187862815 && flag_number8==0){flag_number8 = 1;}// 8
    if(Key == 2194569247 && flag_number9==0){flag_number9 = 1;}// 9
    if(Key == 2184590879 && flag_number10==0){flag_number10 = 1;}//10

    if( flag_number1==1 && 
    	flag_number2==1 && 
    	flag_number3==1 && 
    	flag_number4==1 && 
    	flag_number5==1 && 
    	flag_number6==1 && 
	    flag_number7==1 && 
	    flag_number8==1 && 
	    flag_number9==1 && 
	    flag_number10==1)
    {
    	flag = 1; //перейти до наступного етапу
    }
    
  }
    //========================2 ЕТАП(  )============
    if(flag==1)
    {  
      if(flag_start==0){
        time_start=millis();
        Serial.println(flag);

        digitalWrite(timer_on, HIGH); //запустить таймер
        delay(2000);
        digitalWrite(timer_start, LOW);
        delay(500);
        digitalWrite(timer_start, HIGH);
        
        Serial.println("timer_start");
        mp3_set_serial(Serial3);
        mp3_set_volume(20);
        mp3_play(1);
        b=0;
        flag_start=1;
      }
           //Крутити двигун на 500 кроків
      while(b<610) {
        b++;
        digitalWrite(dirOut, HIGH);
  
        digitalWrite(stepOut, LOW);
  
        delay(9);
  
        digitalWrite(stepOut, HIGH);
      }
      	flag = 2;
      
      /*/==============================2.1 ЗАМИКАННЯ ПРОВОДІВ========================
      char pressed1=customKeypad1.getKey();                               
      if(pressed1=='1' && flag_keypad1 == 0){flag_keypad1=1; mp3_play(6); delay(1000); mp3_play(1); Serial.println('1'); }
      if(pressed1=='2' && flag_keypad2 == 0){flag_keypad2=1; mp3_play(6); delay(1000); mp3_play(1); Serial.println('2'); }
      if(pressed1=='3' && flag_keypad3 == 0){flag_keypad3=1; mp3_play(6); delay(1000); mp3_play(1); Serial.println('3'); }
      if(pressed1=='4' && flag_keypad4 == 0){flag_keypad4=1; mp3_play(6); delay(1000); mp3_play(1); Serial.println('4'); }
      if(pressed1=='5' && flag_keypad5 == 0){flag_keypad5=1; mp3_play(6); delay(1000); mp3_play(1); Serial.println('5'); }
      if(pressed1=='6' && flag_keypad6 == 0){flag_keypad6=1; mp3_play(6); delay(1000); mp3_play(1); Serial.println('6'); }   
      if(flag_keypad1==1 &&flag_keypad2==1 &&flag_keypad3==1 &&flag_keypad4==1 &&flag_keypad5==1 &&flag_keypad6==1)
      {
        flag=2;
        //digitalWrite(timer_start, HIGH); //забираємо високий сигнал з кнопки старт
        Serial.println("OK");
      }*/
    }
     //=======================3 ЕТАП(ПІДКАЗКА З СВІТЛОДІОДІВ)=========
     if(flag==2){
      flag_keypad1=0;
      flag_keypad2=0;
      flag_keypad3=0;
      flag_keypad4=0;
      flag_keypad5=0;
      flag_keypad6=0;
      
      if(flag_number1==1){mp3_play(8);mp3_set_volume(20);code1();flag_mp3_code = 0;time_code = millis();}
      if(flag_number2==1){mp3_play(8);mp3_set_volume(20);code2();flag_mp3_code = 0;time_code = millis();}
      if(flag_number3==1){mp3_play(8);mp3_set_volume(20);code3();flag_mp3_code = 0;time_code = millis();}
      if(flag_number4==1){mp3_play(8);mp3_set_volume(20);code4();flag_mp3_code = 0;time_code = millis();}
      if(flag_number5==1){mp3_play(8);mp3_set_volume(20);code5();flag_mp3_code = 0;time_code = millis();}
      if(flag_number6==1){mp3_play(8);mp3_set_volume(20);code6();flag_mp3_code = 0;time_code = millis();}
      if(flag_number7==1){mp3_play(8);mp3_set_volume(20);code7();flag_mp3_code = 0;time_code = millis();}
      if(flag_number8==1){mp3_play(8);mp3_set_volume(20);code8();flag_mp3_code = 0;time_code = millis();}
      if(flag_number9==1){mp3_play(8);mp3_set_volume(20);code9();flag_mp3_code = 0;time_code = millis();}
      if(flag_number10==1){mp3_play(8);mp3_set_volume(20);code10();flag_mp3_code = 0;time_code = millis();}
      
      while(millis()-time_code<10000)
      {
        admin();
        if(flag_mp3_code==0){
          mp3_set_volume(25);
          mp3_play(7);
          flag_mp3_code = 1;
          string=""; 
        }
        
        char pressed=customKeypad.getKey();
        if(pressed!='\0')
          {
            Serial.println(pressed);
            string += pressed;
            Serial.println(string);
            if(string.length()==6)
            {
              if (flag_number1==1 && string.equals(string_code1)) //правильний код
              {
              	flag_led++;
              	flag_number1=2;
              	flag_number2=2;
              	flag_number3=2;
              	flag_number4=2;
              	flag_number5=2;
              	flag_number6=2;
              	flag_number7=2;
              	flag_number8=2;
              	flag_number9=2;
              	flag_number10=2;
              	digitalWrite(timer_on, LOW);
              	Serial.println("good; timer_stop, LOW");
              	mp3_play(4);flag=0;
              }
              else //неправильний  
                {
                  string=""; 
                  Serial.println("wrong");
                }  
              //delay(1000);
              //digitalWrite(timer_stop, HIGH); //переводимо у вихідне положення контакт скидання таймеру
              //Serial.println("timer_stop, HIGH");
            }
          }
        }
      }
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
    if(Key == 1156837384 && flag_admin2==0){flag_led =10;digitalWrite(timer_on, LOW);time_final=millis(); final(); flag_admin2 = 1;}// 1
    
    if(Key == 1143764744 && flag_admin1==0 && (flag_number1==1 || flag_number2==1 || flag_number3==1 || flag_number4==1 || flag_number5==1 || 
      flag_number6==1 || flag_number7==1 || flag_number8==1 || flag_number9==1 || flag_number10==1)){
      if(flag==1){
        mp3_play(6);
        flag=2;
        delay(2000);
        flag_admin1=1;
      }
      if(flag==2 && flag_admin1==0&& (flag_number1==1 || flag_number2==1 || flag_number3==1 || flag_number4==1 || flag_number5==1 || 
      flag_number6==1 || flag_number7==1 || flag_number8==1 || flag_number9==1 || flag_number10==1)){
        if (flag_number1==1){flag_number1=2;}
        if (flag_number2==1){flag_number2=2;}
        if (flag_number3==1){flag_number3=2;}
        if (flag_number4==1){flag_number4=2;}
        if (flag_number5==1){flag_number5=2;}
        if (flag_number6==1){flag_number6=2;}
        if (flag_number7==1){flag_number7=2;}
        if (flag_number8==1){flag_number8=2;}
        if (flag_number9==1){flag_number9=2;}
        if (flag_number10==1){flag_number10=2;}
        mp3_play(6);
        flag_led++;
        delay(2000);
        digitalWrite(timer_on, LOW);
        mp3_play(4);
        flag=0;
        flag_admin1=1;
      }
    }
    
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
//Функція мигання адресною лентою
void color_bounce() {
   id = id + 1;
    if (id == 3  ) {
      id = 0;
    } 
    
      if(flag_led<1){                            //перша полоса лэнти знизу
        for (int i = 0; i < 3; i++ ) {
        
          if (i == id) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>0 && flag_led<11){
        one_color_green(0,3);
        LEDS.show();                     // отослать команду
      }
      
      if(flag_led<2){                            //2
        for (int i = 3; i < 6; i++ ) {
        
          if (i == id+3) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>1 && flag_led<11){
        one_color_green(3,6);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<3){                            //3
        for (int i = 6; i < 9; i++ ) {
        
          if (i == id+6) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      }if(flag_led>2 && flag_led<11){
        one_color_green(6,9);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<4){                            //4
        for (int i = 9; i < 12; i++ ) {
        
          if (i == id+9) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>3 && flag_led<11){
        one_color_green(9,12);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<5){                            //5
        for (int i = 12; i < 15; i++ ) {
        
          if (i == id+12) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>4 && flag_led<11){
        one_color_green(12,15);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<6){                            //6
        for (int i = 15; i < 18; i++ ) {
        
          if (i == id+15) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>5 && flag_led<11){
        one_color_green(15,18);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<7){                            //7
        for (int i = 18; i < 21; i++ ) {
        
          if (i == id+18) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>6 && flag_led<11){
        one_color_green(18,21);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<8){                            //8
        for (int i = 21; i < 24; i++ ) {
        
          if (i == id+21) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>7 && flag_led<11){
        one_color_green(21,24);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<9){                            //9
        for (int i = 24; i < 27; i++ ) {
        
          if (i == id+24) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      }if(flag_led>8 && flag_led<11){
        one_color_green(24,27);
        LEDS.show();                     // отослать команду
      }
      if(flag_led<10){                            //10
        for (int i = 27; i < 30; i++ ) {
        
          if (i == id+27) {
            leds0[i] = CHSV(thishue, thissat, brightness);
          }
          else {
            leds0[i] = CHSV(0, 0, 0);
          }
        }
      } if(flag_led>9 && flag_led<11){
        one_color_green(27,30);
        LEDS.show();                     // отослать команду
      }
    LEDS.show();
    delay(thisdelay);
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


/*коды на панели
1)238367
2)038195
3)883615
4)114141
5)98358
6)373983
7)384991
8)862815
9)569247
10)590879
 */
void code1(){                   //підказка для першого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number2(1);
  delay(delay_between_number);
  number3(2);
  delay(delay_between_number);
  number8(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number3(1);
  delay(delay_between_number);
  number6(2);
  delay(delay_between_number);
  number7(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code2(){                 //підказка для другого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number0(1);
  delay(delay_between_number);
  number3(2);
  delay(delay_between_number);
  number8(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number1(1);
  delay(delay_between_number);
  number9(2);
  delay(delay_between_number);
  number5(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code3(){                 //підказка для третього коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number8(1);
  delay(delay_between_number);
  number8(2);
  delay(delay_between_number);
  number3(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number6(1);
  delay(delay_between_number);
  number1(2);
  delay(delay_between_number);
  number5(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code4(){                 //підказка для четвертого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number1(1);
  delay(delay_between_number);
  number1(2);
  delay(delay_between_number);
  number4(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number1(1);
  delay(delay_between_number);
  number4(2);
  delay(delay_between_number);
  number1(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code5(){                  //підказка для пятого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number9(1);
  delay(delay_between_number);
  number8(2);
  delay(delay_between_number);
  number3(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number5(1);
  delay(delay_between_number);
  number8(2);
  delay(delay_between_number);
  number3(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code6(){                 //підказка для шостого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number3(1);
  delay(delay_between_number);
  number7(2);
  delay(delay_between_number);
  number3(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number9(1);
  delay(delay_between_number);
  number8(2);
  delay(delay_between_number);
  number3(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code7(){                 //підказка для сьомого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number3(1);
  delay(delay_between_number);
  number8(2);
  delay(delay_between_number);
  number4(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number9(1);
  delay(delay_between_number);
  number9(2);
  delay(delay_between_number);
  number1(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code8(){                   //підказка для восьмоого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number8(1);
  delay(delay_between_number);
  number6(2);
  delay(delay_between_number);
  number2(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number8(1);
  delay(delay_between_number);
  number1(2);
  delay(delay_between_number);
  number5(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code9(){                     //підказка для девятого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number5(1);
  delay(delay_between_number);
  number6(2);
  delay(delay_between_number);
  number9(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number2(1);
  delay(delay_between_number);
  number4(2);
  delay(delay_between_number);
  number7(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}
void code10(){                      ////підказка для десятого коду
  digitalWrite(led1,HIGH);
  delay(delay_between_number);
  number5(1);
  delay(delay_between_number);
  number9(2);
  delay(delay_between_number);
  number0(3);
  delay(delay_between_number);
  digitalWrite(led1,LOW);
  delay(delay_between_number);
  digitalWrite(led2,HIGH);
  delay(delay_between_number);
  number8(1);
  delay(delay_between_number);
  number7(2);
  delay(delay_between_number);
  number9(3);
  delay(delay_between_number);
  digitalWrite(led2,LOW);
}

void lapm_test()
{
  digitalWrite(led1_1, HIGH);
  delay(delay_number);
  digitalWrite(led1_2, HIGH);
  delay(delay_number);
  digitalWrite(led1_3, HIGH);
  delay(delay_number);
  digitalWrite(led1_4, HIGH);
  delay(delay_number);
  digitalWrite(led1_5, HIGH);
  delay(delay_number);
  digitalWrite(led1_6, HIGH);
  delay(delay_number);
  digitalWrite(led2_1, HIGH);
  delay(delay_number);
  digitalWrite(led2_2, HIGH);
  delay(delay_number);
  digitalWrite(led2_3, HIGH);
  delay(delay_number);
  digitalWrite(led2_4, HIGH);
  delay(delay_number);
  digitalWrite(led2_5, HIGH);
  delay(delay_number);
  digitalWrite(led2_6, HIGH);
  delay(delay_number);
  digitalWrite(led3_1, HIGH);
  delay(delay_number);
  digitalWrite(led3_2, HIGH);
  delay(delay_number);
  digitalWrite(led3_3, HIGH);
  delay(delay_number);
  digitalWrite(led3_4, HIGH);
  delay(delay_number);
  digitalWrite(led3_5, HIGH);
  delay(delay_number);
  digitalWrite(led3_6, HIGH);
  delay(delay_number);
  digitalWrite(led1, HIGH);
  delay(delay_number);
  digitalWrite(led2, HIGH);
  delay(2000);
  digitalWrite(led1_1, LOW);
  delay(delay_number);
  digitalWrite(led1_2, LOW);
  delay(delay_number);
  digitalWrite(led1_3, LOW);
  delay(delay_number);
  digitalWrite(led1_4, LOW);
  delay(delay_number);
  digitalWrite(led1_5, LOW);
  delay(delay_number);
  digitalWrite(led1_6, LOW);
  delay(delay_number);
  digitalWrite(led2_1, LOW);
  delay(delay_number);
  digitalWrite(led2_2, LOW);
  delay(delay_number);
  digitalWrite(led2_3, LOW);
  delay(delay_number);
  digitalWrite(led2_4, LOW);
  delay(delay_number);
  digitalWrite(led2_5, LOW);
  delay(delay_number);
  digitalWrite(led2_6, LOW);
  delay(delay_number);
  digitalWrite(led3_1, LOW);
  delay(delay_number);
  digitalWrite(led3_2, LOW);
  delay(delay_number);
  digitalWrite(led3_3, LOW);
  delay(delay_number);
  digitalWrite(led3_4, LOW);
  delay(delay_number);
  digitalWrite(led3_5, LOW);
  delay(delay_number);
  digitalWrite(led3_6, LOW);
  delay(delay_number);
  digitalWrite(led1, LOW);
  delay(delay_number);
  digitalWrite(led2, LOW);
  delay(4000);
}

void number_test()
{
  number1(1);
  delay(1500);
  number2(1);
  delay(1500);
  number3(1);
  delay(1500);
  number4(1);
  delay(1500);
  number5(1);
  delay(1500);
  number6(1);
  delay(1500);
  number7(1);
  delay(1500);
  number8(1);
  delay(1500);
  number9(1);
  delay(1500);
  number0(1);

  delay(2000);

  number1(2);
  delay(1500);
  number2(2);
  delay(1500);
  number3(2);
  delay(1500);
  number4(2);
  delay(1500);
  number5(2);
  delay(1500);
  number6(2);
  delay(1500);
  number7(2);
  delay(1500);
  number8(2);
  delay(1500);
  number9(2);
  delay(1500);
  number0(2);

  delay(2000);

  number1(3);
  delay(1500);
  number2(3);
  delay(1500);
  number3(3);
  delay(1500);
  number4(3);
  delay(1500);
  number5(3);
  delay(1500);
  number6(3);
  delay(1500);
  number7(3);
  delay(1500);
  number8(3);
  delay(1500);
  number9(3);
  delay(1500);
  number0(3);
}

void number1(int a){                //цифра 1
  if(a==1){
    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);  
  }
  if(a==2){
    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW);  
  }
  if(a==3){
    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW);

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW);  
  }
}

void number2(int a){                //цифра 2
  if(a==1){
    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW);

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW); 

    digitalWrite(led1_6,HIGH);
    delay(delay_number);
    digitalWrite(led1_6,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);   
  }
  if(a==2){
    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW);

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW); 

    digitalWrite(led2_6,HIGH);
    delay(delay_number);
    digitalWrite(led2_6,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW); 
  }
  if(a==3){
    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW);

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW); 

    digitalWrite(led3_6,HIGH);
    delay(delay_number);
    digitalWrite(led3_6,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW);  
  }
}
void number3(int a){                //цифра 3
  if(a==1){
    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW);

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW); 

    digitalWrite(led1_6,HIGH);
    delay(delay_number);
    digitalWrite(led1_6,LOW); 
  }
  if(a==2){
        digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW);

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW); 

    digitalWrite(led2_6,HIGH);
    delay(delay_number);
    digitalWrite(led2_6,LOW); 
  }
  if(a==3){
    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW);

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW); 

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW); 

    digitalWrite(led3_6,HIGH);
    delay(delay_number);
    digitalWrite(led3_6,LOW); 
  }
}
void number4(int a){                //цифра 4
  if(a==1){
    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW);

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);  
  }
  if(a==2){
    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW);

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW);  
  }
  if(a==3){
    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW);

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW); 

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW);  
  }
}
void number5(int a){                //цифра 5
  if(a==1){
    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW); 

    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW);

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);  

    digitalWrite(led1_6,HIGH);
    delay(delay_number);
    digitalWrite(led1_6,LOW);  
  }
  if(a==2){
    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW); 

    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW);

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW); 

    digitalWrite(led2_6,HIGH);
    delay(delay_number);
    digitalWrite(led2_6,LOW);  
  }
  if(a==3){
    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW); 

    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW);

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW); 

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW); 

    digitalWrite(led3_6,HIGH);
    delay(delay_number);
    digitalWrite(led3_6,LOW);  
  }
}

void number6(int a){                //цифра 6
  if(a==1){
    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW); 

    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW);

    digitalWrite(led1_6,HIGH);
    delay(delay_number);
    digitalWrite(led1_6,LOW); 

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW);
  }
  if(a==2){
    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW); 

    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW);

    digitalWrite(led2_6,HIGH);
    delay(delay_number);
    digitalWrite(led2_6,LOW); 

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW);
  }
  if(a==3){
    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW); 

    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW);

    digitalWrite(led3_6,HIGH);
    delay(delay_number);
    digitalWrite(led3_6,LOW); 

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW); 

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW);
  }
}
void number7(int a){                //цифра 7
  if(a==1){
    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW);

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);
  }
  if(a==2){
    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW);

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW);  
  }
  if(a==3){
    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW);

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW);
  }
}
void number8(int a){                //цифра 8
  if(a==1){
    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);

    digitalWrite(led1_6,HIGH);
    delay(delay_number);
    digitalWrite(led1_6,LOW); 

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW);

    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW);
      
  }
  if(a==2){
    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW);

    digitalWrite(led2_6,HIGH);
    delay(delay_number);
    digitalWrite(led2_6,LOW); 

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW);

    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW);
  }
  if(a==3){
    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW); 

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW);

    digitalWrite(led3_6,HIGH);
    delay(delay_number);
    digitalWrite(led3_6,LOW); 

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW); 

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW);

    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW);
  }
}
void number9(int a){                //цифра 9
  if(a==1){
    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW);

    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW); 

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);  

    digitalWrite(led1_6,HIGH);
    delay(delay_number);
    digitalWrite(led1_6,LOW); 
  }
  if(a==2){
    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW);

    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW); 

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW);  

    digitalWrite(led2_6,HIGH);
    delay(delay_number);
    digitalWrite(led2_6,LOW); 
  }
  if(a==3){
    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW);

    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW); 

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW);  

    digitalWrite(led3_6,HIGH);
    delay(delay_number);
    digitalWrite(led3_6,LOW); 
  }
}
void number0(int a){                //цифра 0
  if(a==1){
    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 

    digitalWrite(led1_1,HIGH);
    delay(delay_number);
    digitalWrite(led1_1,LOW);

    digitalWrite(led1_2,HIGH);
    delay(delay_number);
    digitalWrite(led1_2,LOW);

    digitalWrite(led1_3,HIGH);
    delay(delay_number);
    digitalWrite(led1_3,LOW);  

    digitalWrite(led1_6,HIGH);
    delay(delay_number);
    digitalWrite(led1_6,LOW); 

    digitalWrite(led1_5,HIGH);
    delay(delay_number);
    digitalWrite(led1_5,LOW);
    
    digitalWrite(led1_4,HIGH);
    delay(delay_number);
    digitalWrite(led1_4,LOW); 
  }
  if(a==2){
    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 

    digitalWrite(led2_1,HIGH);
    delay(delay_number);
    digitalWrite(led2_1,LOW);

    digitalWrite(led2_2,HIGH);
    delay(delay_number);
    digitalWrite(led2_2,LOW);

    digitalWrite(led2_3,HIGH);
    delay(delay_number);
    digitalWrite(led2_3,LOW);  

    digitalWrite(led2_6,HIGH);
    delay(delay_number);
    digitalWrite(led2_6,LOW); 
      
    digitalWrite(led2_5,HIGH);
    delay(delay_number);
    digitalWrite(led2_5,LOW);
    
    digitalWrite(led2_4,HIGH);
    delay(delay_number);
    digitalWrite(led2_4,LOW); 
  }
  if(a==3){
    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 

    digitalWrite(led3_1,HIGH);
    delay(delay_number);
    digitalWrite(led3_1,LOW);

    digitalWrite(led3_2,HIGH);
    delay(delay_number);
    digitalWrite(led3_2,LOW);

    digitalWrite(led3_3,HIGH);
    delay(delay_number);
    digitalWrite(led3_3,LOW);  

    digitalWrite(led3_6,HIGH);
    delay(delay_number);
    digitalWrite(led3_6,LOW); 
      
    digitalWrite(led3_5,HIGH);
    delay(delay_number);
    digitalWrite(led3_5,LOW);
    
    digitalWrite(led3_4,HIGH);
    delay(delay_number);
    digitalWrite(led3_4,LOW); 
  }
}
