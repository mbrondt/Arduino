#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);


void setup()
{
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

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}

void sendSMS(String tlf, String timeStamp, String besked) {
  SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
  delay(100);
  String atTlf = "AT + CMGS = \"" + tlf + "\"";
  SIM900.println(atTlf);
  delay(100);
  String svar = "at: " + timeStamp + "\n" + besked + "\nTegn: ";
  svar.concat(besked.length());
  SIM900.println(svar);
  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000); // give module time to send SMS
  SIM900.println("AT+CMGD=1,4"); // delete all SMS

}

char i_char = 0;
long antalBeskeder = 0;
long i = 0;
String text = "";

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
      Serial.println("Der kom en besked");
      text.trim();

      int pos = text.indexOf('+', 5);
      String tlf = text.substring(pos, pos + 11);
      Serial.println(tlf);

      pos = text.indexOf('/');
      String timeStamp = text.substring(pos-2, pos + 15);
      Serial.println(timeStamp);

      String tekst = text.substring(45);
      Serial.println(tekst);

      if (tekst.startsWith("*") && tekst.endsWith("#")) {
        Serial.println("\nBesked er en kommando som returneres");
        sendSMS(tlf, timeStamp, tekst);
      }
    }
    else
    {
      Serial.println("Echo fra Sim900...");
      Serial.println(text);
    } // End +CMT:
    text = ""; 
    Serial.println("");
  } // End text udfyldt
  delay(10);
}

