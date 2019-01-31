#include <SPI.h>

#include <TM1637.h>

#include <MFRC522.h>

#include <Keypad.h>
#include "FastLED.h"          // библиотека для работы с лентой

#define LED_COUNT 15          // число светодиодов в кольце/ленте

#define LED_DT0 11            // пин, куда подключен DIN ленты
   

int max_bright = 255;         // максимальная яркость (0 - 25)

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
struct CRGB leds0[LED_COUNT];

int brightness = 255;
int thisdelay = 300;          //-FX LOOPS DELAY VAR
int thisstep = 0;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int p = 0;
int id = 0;
int id1 = 0;
int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
/*
int led1 = 22;
int led2 = 23;
int led3 = 24;
int led4 = 22;
int led5 = 23;
int led6 = 24;
int led7 = 22;
int led8 = 23;
int led9 = 24;
int led10 = 24;
int led11 = 22;
int led12 = 23;
int led13 = 24;
int led14 = 22;
int led15 = 23;
int led16 = 24;
int led17 = 22;
int led18 = 23;
int led19 = 24;
int led20 = 24;

int input0 = A0;
int input1 = A1;
int input2 = A2;
int input3 = A3;
int input4 = A4;
int input5 = A5;

int dirOut = 12;
int stepOut = 13;


int lock = 13;

int flag = 0;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad


Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
*/
void setup(){
  Serial.begin(9600);
  /*pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  pinMode(led6,OUTPUT);
  pinMode(led7,OUTPUT);
  pinMode(led8,OUTPUT);
  pinMode(led9,OUTPUT);
  pinMode(led10,OUTPUT);
  pinMode(led11,OUTPUT);
  pinMode(led12,OUTPUT);
  pinMode(led13,OUTPUT);
  pinMode(led14,OUTPUT);
  pinMode(led15,OUTPUT);
  pinMode(led16,OUTPUT);
  pinMode(led17,OUTPUT);
  pinMode(led18,OUTPUT);
  pinMode(led19,OUTPUT);
  pinMode(led20,OUTPUT);
  
  pinMode(lock,OUTPUT);
  digitalWrite(led1,HIGH);
  */
  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость
  LEDS.addLeds<WS2811, LED_DT0, GRB>(leds0, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);          // погасить все светодиоды
  LEDS.show();                     // отослать команду
}
  
void loop(){
  p=1;
  color_bounce1();
  //color_bounce2();
}

void color_bounce1() {
   id = id + 1;
    if (id == 3  ) {
      id = 0;
    }  
  for (int i = 0; i < 3; i++ ) {
      
      if (i == id) {
        leds0[i] = CHSV(thishue, thissat, brightness);
      }
      else {
        leds0[i] = CHSV(0, 0, 0);
      }
    }
    
    for (int i = 3; i < 6; i++ ) {
      
      if (i == id+3) {
        leds0[i] = CHSV(thishue, thissat, brightness);
      }
      else {
        leds0[i] = CHSV(0, 0, 0);
      }
    }
    if(p==0){
      for (int i = 6; i < 9; i++ ) {
        
        if (i == id+6) {
          leds0[i] = CHSV(thishue, thissat, brightness);
        }
        else {
          leds0[i] = CHSV(0, 0, 0);
        }
      }
    }else if(p==1){
      one_color_green(6,9);
      LEDS.show();                     // отослать команду
    }
    LEDS.show();
    delay(thisdelay);
}


void color_bounce() {                    //-m6-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
  
    idex = idex + 1;
    if (idex == LED_COUNT  ) {
      idex = 0;
    }
    int iL1 = adjacent_cw(idex);
    int iL2 = adjacent_cw(iL1);
    int iR1 = adjacent_ccw(idex);
    int iR2 = adjacent_ccw(iR1);
    for (int i = 0; i < LED_COUNT; i++ ) {
      
      if (i == idex) {
        leds0[i] = CHSV(thishue, thissat, brightness);
      }
      else if (i == iL1) {
        leds0[i] = CHSV(thishue, thissat, brightness);
      }
      else if (i == iL2) {
        leds0[i] = CHSV(thishue, thissat, brightness);
      }
      else if (i == iR1) {
        leds0[i] = CHSV(thishue, thissat, brightness);
      }
      else if (i == iR2) {
        leds0[i] = CHSV(thishue, thissat, brightness);
      }
      else {
        leds0[i] = CHSV(0, 0, 0);
      }
    }
    LEDS.show();
    delay(thisdelay);
  
}

//---FIND ADJACENT INDEX CLOCKWISE
int adjacent_cw(int i) {
  int r;
  if (i < LED_COUNT - 1) {
    r = i + 3;
  }
  else {
    r = 0;
  }
  return r;
}

//---FIND ADJACENT INDEX COUNTER-CLOCKWISE
int adjacent_ccw(int i) {
  int r;
  if (i > 0) {
    r = i - 3;
  }
  else {
    r = LED_COUNT - 1;
  }
  return r;
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

