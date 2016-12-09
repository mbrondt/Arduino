#include <dht.h>
dht DHT;
#define DHT22_PIN 6
#define DHT11_PIN 7

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

void setup() {
  lcd.createChar(0, smiley);
  lcd.begin(16, 2);  
  lcd.write(byte(0));
delay(2000);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(8,0);
  lcd.print("DHT");
  lcd.setCursor(4,1);
  lcd.print("Test Program");
  lcd.setCursor(0,3);
  lcd.print("Lib version:");
  lcd.setCursor(13,3);
  lcd.print(DHT_LIB_VERSION);
  delay(2000);
  lcd.clear();


  lcd.setCursor(0,0);
  lcd.print((char)0B11100001);
  lcd.setCursor(6,1);
  lcd.print((char)223);
  lcd.setCursor(9,0);
  lcd.print("Gr");
  lcd.write(byte(0));
  lcd.print(" Temp");
  lcd.setCursor(16,1);
  lcd.print((char)223);
  lcd.setCursor(9,2);
  lcd.print("l:");
  lcd.setCursor(16,2);
  lcd.print((char)223);
  lcd.setCursor(9,3);
  lcd.print("h:");
  lcd.setCursor(16,3);
  lcd.print((char)223);
}

char rulle[] = {
  'M','B','r','o','n','d','t'};
float lowT = 99.9;
float highT = -99.9;
int rulleId = 0;
long previousMillis = 0;

void loop() {
  {
    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis > 1000) {
      previousMillis = currentMillis;


      lcd.setCursor(2,1);
      callDHT11();
      lcd.print(DHT.temperature, 1);
      callDHT22();
      lcd.setCursor(12,1);
      lcd.print(DHT.temperature, 1);
      if (DHT.temperature < lowT) {
        lowT = DHT.temperature;
      }
      if (DHT.temperature > highT) {
        highT = DHT.temperature;
      }
      lcd.setCursor(12,2);
      lcd.print(lowT,1);
      lcd.setCursor(12,3);
      lcd.print(highT,1);
    }    
    lcd.setCursor(0,3);
    lcd.print(rulle[rulleId++]);
    if (rulleId >= sizeof(rulle)) {
      rulleId = 0;
    }
    delay(1000);
  }  
}

void callDHT22() {
  int chk = DHT.read22(DHT22_PIN);
  switch (chk)
  {
  case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
  case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
  case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
  default: 
    Serial.print("Unknown error,\t"); 
    break;
  }

}

void callDHT11() {
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
  case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
  case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
  case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
  default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
}






