#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup() {
  lcd.begin(20,4);  
  lcd.home();
}

long count = 0;
void loop() { 
  int xpos = random(19);
  int ypos = random(4);
  lcd.setCursor(xpos,ypos);
  lcd.print(count++);
  delay(1000);
  lcd.clear();
//  lcd.setCursor(xpos,ypos);
//  lcd.print("     ");
}

