#include <LiquidCrystal.h>

#define SELECTPIN 2
#define UP 7
#define DOWN 6
#define RIGHT 3
#define LEFT 0

LiquidCrystal lcd(12, 11, 5, 4, 10, 8);

int hours = 0;
int minutes = 0;
int seconds = 0;
boolean AM = true;
String cl = "AM";
String clockNow = "";
int index = -1;
boolean setMode = false;

byte arrow[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100
};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(SELECTPIN, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  clockNow = String(hours / 10)+String( hours % 10 )+":"+String(minutes / 10)+String( minutes % 10 )+":"+String(seconds / 10)+String( seconds % 10 )+":"+cl;
  lcd.print(clockNow);
  attachInterrupt(digitalPinToInterrupt(SELECTPIN),selection,RISING );  //  function for creating external interrupts at pin2 on Rising (LOW to HIGH)
  attachInterrupt(digitalPinToInterrupt(RIGHT),incrementIndex,RISING );  //  function for creating external interrupts at pin2 on Rising (LOW to HIGH)
}

void loop() {
  if (!setMode){
    delay(100);
    seconds++;
    if(seconds > 60){
      seconds = 0;
      minutes++;
    }
    if(minutes > 60){
      minutes = 0;
      hours ++;
    }
    if(hours > 12){
      hours = 0;
      AM = !AM;
      if(AM)
        cl = "AM";
      else 
        cl = "PM";
    }
    displayClockNow();  
  }
  if(setMode == true){
    SetMode();
  }
}

void displayClockNow(){
  clockNow = String(hours / 10)+String( hours % 10 )+":"+String(minutes / 10)+String( minutes % 10 )+":"+String(seconds / 10)+String( seconds % 10 )+":"+cl;
  lcd.clear();
  lcd.print(clockNow);
}

void selection(){
  setMode = true;
  displayClockNow();
}

void arrowDisplay(){
  lcd.createChar(0,arrow);
  lcd.home();
  if (index == 0){
    displayClockNow();
    lcd.setCursor(0,2);
    lcd.write((byte)0);
  }else if (index == 1){
    displayClockNow();
    lcd.setCursor(1,2);
    lcd.write((byte)0);
  }else if (index == 2 || index == 3){
    displayClockNow();
    lcd.setCursor(3,2);
    lcd.write((byte)0);
  }else if (index == 4){
    displayClockNow();
    lcd.setCursor(4,2);
    lcd.write((byte)0);
  }else if (index == 5 || index == 6){
    displayClockNow();
    lcd.setCursor(6,2);
    lcd.write((byte)0);
  }else if (index == 7){
    displayClockNow();
    lcd.setCursor(7,2);
    lcd.write((byte)0);
  }else if (index == 8 || index == 9 || index == 10){
    displayClockNow();
    lcd.setCursor(9,2);
    lcd.write((byte)0);
  }
}

void incrementIndex(){
//  if(!setMode) return;
  index++;
  arrowDisplay();
  if (index > 10){
    index = -1;
    setMode = false;
    return;
  }  
}
void decrementIndex(){
  //if(!setMode) return;
  index--;
  arrowDisplay();
  if (index <0){
    index = 0;
  }
}


void SetMode(){
//  if(!setMode) return;
 if (digitalRead(UP) == 0){
    increment();
    displayClockNow();
   arrowDisplay();
   	while(digitalRead(UP) == 0);
  }
  if (digitalRead(DOWN) == 0){
    decrement();
    displayClockNow();
    arrowDisplay();
    while(digitalRead(DOWN) == 0);
  }
  if (digitalRead(LEFT) == 0){
    decrementIndex();
    displayClockNow();
    arrowDisplay();
   	while(digitalRead(LEFT) == 0);
  }
}

void increment(){
  
  
  if (index == 8 || index == 9 || index == 10) AM = !AM;
  else if (index == 7) seconds++;
  else if (index == 5 || index == 6) seconds+=10;
  else if (index == 4) minutes++;
  else if (index == 2 || index == 3) minutes+=10;
  else if (index == 1) hours++;
  else if (index == 0) hours+=10;
    
  Serial.println(index);
    
  checkUP();
  arrowDisplay();
  
}

void decrement(){
  
  if (index == 8 || index == 9 || index == 10) AM = !AM;
  else if (index == 7) seconds--;
  else if (index == 5 || index == 6) seconds-=10;
  else if (index == 4) minutes--;
  else if (index == 2 || index == 3) minutes-=10;
  else if (index == 1) hours--;
  else if (index == 0) hours-=10;
  
  checkDown();
  arrowDisplay();
}

void checkDown(){
  if (seconds<0) seconds = 0;
  else if (hours<0)  hours = 0;
  else if (minutes<0) minutes = 0;
  
  AM?cl = "AM":cl = "PM";
}
void checkUP(){
  if (seconds>60) seconds = 0;
  else if (hours>12)  hours = 0;
  else if (minutes>60) minutes = 0;

  AM?cl = "AM":cl = "PM";
}
