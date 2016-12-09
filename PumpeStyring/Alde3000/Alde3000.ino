//#include <DHT.h>

#include <dht.h>

dht DHT11;
dht DHT22;

#define DHT22_PIN 6
#define DHT11_PIN 7

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myServo;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte termo[8] = {B00100, B01010, B01010, B01010, B01010, B10101, B10001, B01110};
byte grad[8] = {B11100, B10100, B11100, B00000, B00000, B00000, B00000,};
byte oe[8] = {B00000, B00000, B01110, B10011, B10101, B11001, B01110, B00000};
byte g[8] = {B00000, B00000, B01111, B10001, B10001, B01111, B00001, B01110};
byte hPunktum[8] = {B00000, B00000, B00000, B00000, B00000, B00011, B00011, B00000};

boolean prik = false;
boolean manualControl = false;


void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.createChar(0, termo);
  lcd.write(byte(0));
  lcd.createChar(1, grad);
  lcd.write(byte(1));
  lcd.createChar(2, oe);
  lcd.write(byte(2));
  lcd.createChar(3, g);
  lcd.write(byte(3));
  lcd.createChar(4, hPunktum);
  lcd.write(byte(4));

  while (! Serial);
  Serial.println("Enter position to set Servo: 1 to 5");

  myServo.attach(9);
  myServo.write(135);

  skrivAutLedetekster();

  delay(1000);

}


void loop() {
  float frem = 0.0;
  float retur = 0.0;
  int position = 0;

  if (Serial.available()) {
    char ch = manualOverwrite();
    Serial.write(ch);
    if (ch >= '1' && ch <= '5') {
      manualControl = true;
      Serial.println("manualControl on");
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Alde Compact 3000");

      lcd.setCursor(1, 2);
      lcd.print("Pumpe (manuel) ");
      lcd.write(ch);
      lcd.setCursor(7, 3);
      position = ch - '0';
      lcd.print((position - 1) * 33);
      lcd.print(" ");
      lcd.write(byte(3));
      lcd.print("r.");

    }
    if (ch == 'x') {
      manualControl = false;
      Serial.println("manualControl off");
      skrivAutLedetekster();
    }
  }

  if (!manualControl) {
    // Fremløb
    callDHT11();
    frem = DHT11.temperature;
    if (frem <= 99.9) {
      lcd.setCursor(13, 1);
      lcd.print(frem, 1);
      lcd.write(byte(1));
      lcd.print("  ");
    }
    delay(1000);

    // Returløb
    callDHT22();
    retur = DHT22.temperature;
    if (retur <= 99.9) {
      lcd.setCursor(12, 2);
      lcd.print("       ");
      lcd.setCursor(12, 2);
      lcd.print((frem - retur) * -1, 1);
      lcd.write(byte(1));
    }
    delay(1000);

    // Fugtighed
    if ((DHT22.humidity, 0) < 100) {
      //lcd.setCursor(6, 3);
      //lcd.print("    ");
      lcd.setCursor(7, 3);
      lcd.print(DHT22.humidity, 0);
      //lcd.print("%");
    }

    //  delay(1000);

    // Skal pumpen justeres (spring over ved fejllæsning
    if (frem <= 99 && retur <= 99) {
      position = findTempDiff(frem, retur);
    }
    lcd.setCursor(17, 3);
    lcd.print(position);
  }

  heartBeat();
  justerPumpe(position);

  // Ro på loop
  delay(2000);
}


void justerPumpe(int pos) {
  if (pos >= 1 && pos <= 5) {
    int nuvPos = myServo.read();
    int nyPos = (pos - 1) * 33;
    if (nyPos > nuvPos) {
      for (int idx = nuvPos; idx <= nyPos; idx++) {
        myServo.write(idx);
        delay(50);
      }
    } else {
      for (int idx = nuvPos; idx >= nyPos; idx--) {
        myServo.write(idx);
        delay(50);
      }
    }

    Serial.print("Servo sat til ");
    Serial.print(myServo.read());
    Serial.println(" gr.");
  }
}

char manualOverwrite() {
  int idx = 0;
  char ch = Serial.read();
  Serial.write(ch);
  if (ch >= '1' && ch <= '5')
  {
    Serial.print("Servo saettes til ");
    return ch;
  }
  if (ch == 'x')
  {
    Serial.println("Manual overwrite exited");
    return ch;
  }
}

int findTempDiff(int frem, int retur) {
  Serial.print("frem ");
  Serial.print(frem);
  Serial.print("\tretur ");
  Serial.print(retur);

  int diff = (frem - retur);
  Serial.print("\tdiff ");
  Serial.println(diff);
  if (diff < 2) {
    return 1;
  }
  if (diff >= 2  && diff < 6)  {
    return 2;
  }
  if (diff >= 6  && diff < 11)  {
    return 3;
  }
  if (diff >= 11  && diff < 16)  {
    return 4;
  }
  if (diff >= 16) {
    return 5;
  }
  return 0;
}

void skrivAutLedetekster() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Alde Compact 3000");

  lcd.setCursor(3, 1);
  lcd.print("freml");
  lcd.write(byte(2));
  lcd.print("bs");
  lcd.write(byte(0));
  lcd.print(" ");

  lcd.setCursor(2, 2);
  lcd.print("returdif");
  lcd.write(byte(0));
  lcd.print(" ");

  lcd.setCursor(2, 3);
  lcd.print("fu");
  //g
  lcd.write(byte(3));
  lcd.print("t ");

  lcd.setCursor(11, 3);
  lcd.print("pumpe ");
}

void callDHT22() {
  int chk = DHT22.read22(DHT22_PIN);
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
  int chk = DHT11.read11(DHT11_PIN);
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

void heartBeat() {
  //   Serial.print(prik);
  if (prik == false) {
    lcd.setCursor(0, 0);
    lcd.print(".");
    lcd.setCursor(18, 0);
    lcd.print(" ");
    prik = true;
  } else {
    lcd.setCursor(18, 0);
    lcd.write(byte(4));
    lcd.setCursor(0, 0);
    lcd.print(" ");
    prik = false;
  }
}




