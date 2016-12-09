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

    if (text.startsWith("+CMT:")) { // Modtaget SMS af mod SIM900
      int pos = text.indexOf("\"", 25);
      //      Serial.println(text.substring(pos + 1));
      String cmd = text.substring(pos+1, pos+2);

      if (cmd == "O") { // Operator
        SIM900.print("AT+COPS?\r");
        delay(1000);
        String svar = readGSM();
        if (text.indexOf("+COPS:") > 0) {
          teleOperator(iv_Tlf, text);
        } 
        else {
          Serial.println("Fejl i svar fra SIM900 (COPS)");
        }
      } 
      else if (cmd == "S") { // Strength
        SIM900.print("AT+CSQ\r");
      } 
      else if (cmd == "T") { // TimeStamp
        SIM900.print("AT+CCLK?\r");
      } 
      else {
        Serial.println("Modtaget: " + cmd + " Forventet: S,O,T");
      }
      iv_Tlf = gemAfsenderTlf(text);
      iv_ModtagetTS = gemAfsenderTS(text);

    } 
    else { // Det er ikke en SMS/CMT
      Serial.println();
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
      if (currentMillis > 30000) { //Hvis ikke vi har synk så skal GSM genstartes
        Serial.println("Opstart fejlet, genstart af SIM900");
        powerUpSIM900();
      }
      Serial.println("Syncronizing Time");
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

  if(currentMillis - previous60 > (10*60000)) { // 1 time
    previous60 = currentMillis;
    if (isSIM900Running) {
      sendSMS("+4522947000", "SIM900 Powered down");
      SIM900power();
      Serial.println("SIM900 Powered down");
      isSIM900Running = false;
    } 
    else {
      powerUpSIM900();
      sendSMS("+4522947000", "SIM900 Powered up");
    }
  } 
}


