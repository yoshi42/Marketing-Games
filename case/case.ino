/*
Коды кейсы:
1) 1311410000 //
2) 1210181210 //
3) 1216173070 //
4) 1916135120 //
5) 1462011313 //
6) 1920615100 //
7) 1210511329 //
8) 1416273000 //
9) 2561820780 //
10) 1912101500 //
 */

#include <Keypad.h>

int led1 = 10;
int led2 = 11;
int led3 = 12;
#define lock A0
int led_internal = 13;

int buzzer = 15;
int flag = 0;
int flag_true = 0;
int flag_false = 0;

char char1 = '1';
char char2 = '9';
char char3 = '1';
char char4 = '2';
char char5 = '1';
char char6 = '0';
char char7 = '1';
char char8 = '5';
char char9 = '0';
char char10 = '0';

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

void setup(){
  Serial.begin(9600);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(lock,OUTPUT);
  pinMode(led_internal, OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(led_internal, LOW);
  
  digitalWrite(lock,LOW);
  digitalWrite(led1,HIGH);
}
  
void loop(){
  char pressed=customKeypad.getKey();
  
 if(flag_true==0){                       //первый символ кода
   if(pressed==char1){
      flag_true=1;
      
   }
 }
  if(flag_true==1){                     //второй
   if(pressed==char2){
      flag_true=2; 
   }
 }
 if(flag_true==2){                     //третий
   if(pressed==char3){
      flag_true=3; 
   }
 }
 if(flag_true==3){                     //четвертый
   if(pressed==char4){
      flag_true=4; 
   }
 }
 if(flag_true==4){                     //5
   if(pressed==char5){
      flag_true=5; 
   }
 }
 if(flag_true==5){                     //6
   if(pressed==char6){
      flag_true=6; 
   }
 }
 if(flag_true==6){                     //7
   if(pressed==char7){
      flag_true=7; 
   }
 }
 if(flag_true==7){                     //8
   if(pressed==char8){
      flag_true=8; 
   }
 }
 if(flag_true==8){                     //9
   if(pressed==char9){
      flag_true=9; 
   }
 }
 if(flag_true==9){                     //10
   if(pressed==char10){
      flag_true=10; 
   }
 }
  if(pressed!='\0'){
    flag++;    
    
    Serial.println(pressed);
    digitalWrite(led2, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(70);
    digitalWrite(led2, LOW);
    digitalWrite(buzzer, LOW);
    
  }
 if(flag==10){
  if(flag_true==10){
      digitalWrite(lock,HIGH);
      digitalWrite(led2,HIGH);
      
      digitalWrite(buzzer, HIGH);
      delay(50);
      digitalWrite(buzzer, LOW);
      delay(50);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      
      delay(2000);
      digitalWrite(lock,LOW);
      digitalWrite(led2,LOW); 
      flag = 0;
      flag_true = 0;
      flag_false = 0;
  }
  else if(flag_true!=10){
      digitalWrite(led3,HIGH);
      
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      delay(400);
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      delay(400);
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      
      digitalWrite(led3,LOW);
      flag = 0;
      flag_true = 0;
      flag_false = 0;
  }
 }
}
