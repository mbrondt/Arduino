#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup() {
  initDK();
  lcd.home();
  for (int id = 0; id < 3; id++) {
    lcd.write(byte(id));
  }
}

void loop() { 
}

void initDK() {
  byte ae[8] = {
    B00000,B00000,B11010,B00101,B01110,B10100,B01011,B000000 };
  byte oe[8] = {
    B00000,B00000,B01110,B10011,B10101,B11001,B01110,B000000  };
  byte aa[8] = {
    B00110,B00000,B01110,B00001,B01111,B10001,B01111,B000000  };
  lcd.begin(20,4);  
  lcd.createChar(0, ae);
  lcd.createChar(1, oe);
  lcd.createChar(2, aa);
}



