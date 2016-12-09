#include <SoftwareSerial.h>
#include <dht.h>

SoftwareSerial SIM900(7, 8);
dht DHT;

#define DHT22_PIN 10
#define LED_PIN 13
#define RELAY_PIN 11


void setup()
{
  pinMode(LED_PIN, OUTPUT); 
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);

  Serial.begin(9600); // for serial monitor
  Serial.println("\nSketch initiated");
  SIM900.begin(19200); // for GSM shield
  SIM900power();  // turn on shield
  delay(20000);  // give time to log on to network.

  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
  Serial.println("Sim900 Started");
}


char i_char = 0;
long i = 0;
String SMS = "";
unsigned long previousMillis = 0;
void loop()
{
  // digitalWrite(LED_PIN, HIGH);
  while (SIM900.available() > 0) {
    i_char = SIM900.read(); 
    i++;
    if (i < 52) { //Ingen af de indledende beskeder
    } 
    if (i == 52) {
      Serial.println("Sim900 Ready...\n");
    }
    if (i > 52) {
      if (i_char != 10 && i_char != 13) {
        SMS += i_char;
      } 
    }
  }
  //digitalWrite(LED_PIN, LOW);   // turn the LED on (HIGH is the voltage level)

  String svarTekst;
  if (SMS.length() > 0) {
    SMS.trim();

    if (SMS.startsWith("+CMT:")) {

      skrivAfsender(SMS);
      String beskedDel = SMS.substring(45); // fra tegn 45 og resten af linien
      beskedDel.trim();

      if (beskedDel.startsWith("*") && beskedDel.endsWith("#")) {
        Serial.println("\nBesked er en kommando som behandles");

        if (beskedDel == "*temperature#" || beskedDel == "*humidity#") {
          svarTekst = getDHT22Data(beskedDel);
        } 
        svarTekst = setHeat(beskedDel);
        
        if (beskedDel == "*strength#") {
          SIM900.print("AT+CSQ\r");
        } 
        if (beskedDel == "*operator#") {
          SIM900.print("AT+COPS?\r"); 
        }
      } //End på operationer

      if (SMS.endsWith("ERROR")) {
        Serial.print("SIM900: ukendt kommando: ");
        Serial.println(SMS);
      }

      if (svarTekst.length() > 0) {
        Serial.println(svarTekst);
      }
      svarTekst += "\nTegn: " + beskedDel.length();
    } // End of +CMT: (TextMessage)

    if (SMS.indexOf("+COPS:") > 0) {
      int pos = SMS.indexOf("\"");
      Serial.print("Operator: ");
      Serial.println(SMS.substring(pos + 1, SMS.length() - 3));
    }
    if (SMS.indexOf("+CSQ:") > 0) {
      int pos = SMS.indexOf(":");
      Serial.print("Signal Strength: ");
      Serial.println(SMS.substring(pos + 2, SMS.length() - 4));
    }

    //sendSMS(tlf, timeStamp, svarTekst);
    svarTekst = "";

  } // End på SMS
  SMS = "";
  // End SMS udfyldt

  //Kald for at opdatare min og max
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > 600000) {
    previousMillis = currentMillis;
    getDHT22Data("*temperature#");
  }

  delay(1);
}














