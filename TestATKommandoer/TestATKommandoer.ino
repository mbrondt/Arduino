#include <Time.h>


#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);
#define LED_PIN 13

void setup()
{
  pinMode(LED_PIN, OUTPUT); 
  Serial.begin(19200);
  powerUpSIM900(); 
}


unsigned long previous10 = 0;
unsigned long previous60 = 0;
boolean isSIM900Running = false;
String iv_Tlf;
String iv_ModtagetTS;
void loop()
{
  String text = readGSM();

  if (text.length() > 0) {
    //Serial.println(text);
    text.trim();
//      Serial.println(text);

    if (text.startsWith("+CMT:")) { // Modtaget SMS af mod SIM900
      int pos = text.indexOf("\"", 25);
      //      Serial.println(text.substring(pos + 1));
      String cmd = text.substring(pos+1, pos+2);

      if (cmd == "B") { // Boot SIM900
        delay(10000);
        boot();
      } 
      else if (cmd == "Q") { // Power down
        delay(10000);
        powerDown();
      } 
      else if (cmd == "O") { // Operator
        SIM900.print("AT+COPS?\r");
      } 
      else if (cmd == "S") { // Strength
        SIM900.print("AT+CSQ\r");
      } 
      else if (cmd == "T") { // TimeStamp
        SIM900.print("AT+CCLK?\r");
      } 
      else {
        Serial.println("Modtaget: " + cmd + " Forventet: (B)oot,(Q)uit,(O)perator,(S)trength,(T)imestamp");
      }
      iv_Tlf = gemAfsenderTlf(text);
      iv_ModtagetTS = gemAfsenderTS(text);

    } 
    else { // Det er ikke en SMS/CMT
      Serial.println();
      if (text.indexOf("+CPIN: NOT INSERTED")) {
        Serial.println("Simkort mangler");
      } 
      if (text.indexOf("+COPS: 0")) {
        Serial.println("Ingen operator");
      } 
      if (text.indexOf("+COPS:") > 0) {
        teleOperator(iv_Tlf, text);
      }
      if (text.indexOf("+CCLK:") > 0) {
        timeStamp(iv_Tlf, text);
      }
      if (text.indexOf("+CSQ:") > 0) {
        strength(iv_Tlf, text);
      }
    }
  } // Intet output fra readGSM

  unsigned long currentMillis = millis();
  if(currentMillis - previous10 > 10000) { // Vis kl hvert 10. sek.
    previous10 = currentMillis;
    if (timeStatus() == 0) {
      if (currentMillis > 30000 ) { //Hvis ikke vi har synk så skal GSM genstartes
        Serial.println("Opstart fejlet, genstart af SIM900");
        powerUpSIM900();
        sendSMS("+4522947000", "Opstart fejlet eller ingen konsol, SIM900 startet op");
      }
      Serial.println("Initial syncronizing time");
      SIM900.print("AT+CCLK?\r");
    } 
    else {
      time_t t = now();
      Serial.print(hour(t)); 
      Serial.print(".");  
      Serial.print(minute(t)); 
      Serial.print("."); 
      Serial.println(second(t));
    }
  }

  if(currentMillis - previous60 > (10*60000)) { // 60000 = 1 min.
    previous60 = currentMillis;
    if (isSIM900Running) {
      sendSMS("+4522947000", "SIM900 Power down");
      powerDown();
      Serial.println("SIM900 Power down");
      isSIM900Running = false;
    } 
    else {
      powerUpSIM900();
      sendSMS("+4522947000", "SIM900 Powered up");
    }
  } 
}

//CLTS = time stamp
//CSQ = signal strengt
//AT+COPS? Get udbyder
//AT+CMGL="ALL" List alle sms'er

























