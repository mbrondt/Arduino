#include <SoftwareSerial.h>
#include <dht.h>

SoftwareSerial SIM900(7, 8);
dht DHT;

#define DHT22_PIN 10
#define LED_PIN 13


void setup()
{
  pinMode(LED_PIN, OUTPUT);     
  Serial.begin(19200); // for serial monitor
  SIM900.begin(19200); // for GSM shield
  SIM900power();  // turn on shield
  Serial.println("Sim900 initiated");
  delay(20000);  // give time to log on to network.

  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
}


char i_char = 0;
long i = 0;
String text = "";
unsigned long previousMillis = 0;
void loop()
{
  while (SIM900.available() > 0) {
    i_char = SIM900.read(); 
    i++;
    if (i < 50) { //Ingen af de indledende beskeder
    } 
    if (i == 50) {
      Serial.println("Sim900 Ready...\n");
    }
    if (i > 50) {
      if (i_char != 10 && i_char != 13) {
        text += i_char;
      } 
    }
  }

  if (text.length() > 0) {
    if (text.startsWith("+CMT:")) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Der kom en besked");
      text.trim();

      int pos = text.indexOf('+', 5);
      String tlf = text.substring(pos, pos + 11);
      Serial.println(tlf);

      pos = text.indexOf('/');
      String timeStamp = text.substring(pos-2, pos + 15);
      Serial.println(timeStamp);

      String tekst = text.substring(45); // fra tegn 45 og resten af linien
      tekst.trim();
      Serial.println(tekst);

      if (tekst.startsWith("*") && tekst.endsWith("#")) {
        String temp;
        Serial.println("\nBesked er en kommando som behandles");

        if (tekst == "*temperature#") {
          temp = getDHT22Data('T');
        } 
        else if (tekst == "*humidity#") { 
          temp = getDHT22Data('H');
        } 
        else {
          temp = tekst + "\nTegn: " + tekst.length();
        }
        sendSMS(tlf, timeStamp, temp);
      }
      digitalWrite(LED_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
    }
    else
    {
      Serial.println("Echo fra Sim900...");
      Serial.println(text);
    } // End +CMT:
    text = ""; 
    Serial.println("");
  } // End text udfyldt
  
  //Kald for at opdatare min og max
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > 600000) {
    previousMillis = currentMillis;
    getDHT22Data('T');
  }

  delay(10);
}



