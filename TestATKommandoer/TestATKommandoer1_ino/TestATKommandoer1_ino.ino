#include <Time.h>


#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);
#define LED_PIN 13

void setup()
{
  pinMode(LED_PIN, OUTPUT); 
  Serial.begin(9600);
  powerUpSIM900(); 
}



//CLTS = time stamp
//CSQ = signal strengt
//AT+COPS? Get udbyder
//AT+CMGL="ALL" List alle sms'er




















