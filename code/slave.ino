//Slave
#include <LiquidCrystal.h>
#include <Wire.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char asked;

//Initial setup
void setup() {
 lcd.begin(16,2);
 Wire.begin(8);
 Wire.onReceive(receiveEvent);
 Serial.begin(9600);
}

void loop() {
  //do nothing
}

//When requested set to display time/delay/clear
void receiveEvent(int howMany){
  lcd.setCursor(0, 0);
  asked = Wire.read();
  delay(10);
  if(asked == 'd'){
    lcd.print("Time:");
    setData(howMany);
  }
  if(asked == 'c'){
    lcd.clear();
  }
  if(asked == 'e'){
    lcd.print("Delay:");
    setDelay(howMany);
  }
}

//Change to time mode
void setData(int howMany){
  lcd.setCursor(0, 1);
  long x = (Wire.read() << 8) | Wire.read();
  float time = (float)x/1000;
  lcd.print(time);
  lcd.print("s   ");
}

//Change to delay mode
void setDelay(int howMany){
  lcd.setCursor(0, 1);
  int x = (Wire.read() << 8) | Wire.read();
  float delay = (float)x/1000;
  lcd.print(delay);
  lcd.print("s   ");
}