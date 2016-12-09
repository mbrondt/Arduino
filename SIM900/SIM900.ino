#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // configure software serial port
void setup()
{
  Serial.begin(9600);
  SIM900.begin(19200);               
  SIM900power();  
  delay(20000);  // give time to log on to network. 
  Serial.println("pass network delay (20000)");
  
}
 
void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
 pinMode(9, OUTPUT); 
 digitalWrite(9,LOW);
 delay(1000);
 digitalWrite(9,HIGH);
 delay(2000);
  Serial.println("pin 9 sat hoj i 2 sek");
 digitalWrite(9,LOW);
 delay(3000);
  Serial.println("pin 9 sat lav i 3 sek");

}
 
void callSomeone()
{
  SIM900.println("ATD + +4522947000;"); // dial US (212) 8675309
  delay(100);
  SIM900.println();
  delay(30000);            // wait for 30 seconds...
  SIM900.println("ATH");   // hang up
}
 
void loop()
{
  callSomeone(); // call someone
  SIM900power();   // power off GSM shield
  do {} while (1); // do nothing
}
