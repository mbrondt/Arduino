#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);

void setup() {
  // initialize serial:
  Serial.begin(19200); 
  SIM900.begin(19200); // for GSM shield
  SIM900power();  // turn on shield
  Serial.println("Sim900 initiated");
  delay(15000);  // give time to log on to network.

  //SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  //delay(100);
  //SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  //delay(100);
  SIM900.print("AT+COPS?\r");
  delay(100);
  SIM900.print("AT+CSQ\r");
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

void send(String kommando) {
  Serial.println("Sender...");
  kommando += "\r";
  SIM900.print(kommando);
  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000); // give module time to send SMS
}

char input = 0;
String text = "";
void loop() {
  while (Serial.available() > 0) {
     input = Serial.read();
    
    if (input != 13 && input != 10) {
      text += input;
    }
  }
  
  if (text.length() > 0) {
    text.trim();
    Serial.println(text);
    send(text);
    text = "";
  }
  
  while (SIM900.available() != 0) {
    Serial.write(SIM900.read());
  }
  delay(20);
}
