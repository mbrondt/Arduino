#include <SoftwareSerial.h>
#include <dht.h>

SoftwareSerial SIM900(7, 8);
dht DHT;

#define DHT22_PIN 10
#define LED_PIN 13
#define RELAY_PIN 11

boolean isSIM900Running = false;
unsigned long previous10 = 0;
unsigned long previous60 = 0;


void loop()
{
  // digitalWrite(LED_PIN, HIGH);
  String svarText = readGSM();
  //digitalWrite(LED_PIN, LOW);   // turn the LED on (HIGH is the voltage level)

  if (svarText.length() > 0) {
    svarText.trim();

    if (svarText.startsWith("+CMT:")) {

      String tlf = getTlf(svarText);
      String tS = getAfsendtTS(svarText);
      String beskedDel = svarText.substring(45); // fra tegn 45 og resten af linien
      beskedDel.trim();

      if (svarText.endsWith("ERROR")) {
        Serial.print("SIM900: ukendt kommando: ");
        Serial.println(svarText);
      }

    } // End of +CMT: (Modtaget SMS)

    if (svarText.indexOf("+COPS:") > 0) {
      int pos = svarText.indexOf("\"");
      Serial.print("Operator: ");
      Serial.println(svarText.substring(pos + 1, svarText.length() - 3));
    }
    if (svarText.indexOf("+CSQ:") > 0) {
      int pos = svarText.indexOf(":");
      Serial.print("Signal Strength: ");
      Serial.println(svarText.substring(pos + 2, svarText.length() - 4));
    }

    //sendSMS(tlf, timeStamp, svarTekst);
    svarText = "";

  } // End på tekst fra readGSM

  unsigned long currentMillis = millis();
  if(currentMillis - previous60 > (10*60000)) { // 1 time
    previous60 = currentMillis;
    if (isSIM900Running) {
      //      sendSMS("+4522947000", "SIM900 Powered down");
      SIM900toggle();
      Serial.println("SIM900 Powered down");
      isSIM900Running = false;
    } 
    else {
      Serial.println("SIM900 Power up");
      SIM900Start();
      //      sendSMS("+4522947000", "SIM900 Powered up");
    }
  } 
}
















