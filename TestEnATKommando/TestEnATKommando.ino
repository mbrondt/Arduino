#include <SoftwareSerial.h>

SoftwareSerial SIM900(7,8);

void setup()
{
  Serial.begin(19200);
  powerUpSIM900(); 
}

int cc = 0;
void loop()
{
  char i_char = 0;
  while (SIM900.available() > 0) {
    i_char = SIM900.read(); 
    Serial.print(i_char);
    delay(1);
  }
  if (cc == 0) {
    SIM900.print("AT+GSV\r");
  }
  cc++;
  //  SIM900.print("AT+CSQ\r");
  //  SIM900.print("AT+CCLK?\r");
delay(10);

}

//IPR=? = Supported baudrate
//IPR=x = set baudrate to x
//CLTS = time stamp
//CSQ = signal strengt
//AT+COPS? Get udbyder
//AT+CMGL="ALL" List alle sms'er
//AT+CPAS = Phone Activity status





















