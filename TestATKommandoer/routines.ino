void haltSystem() {
  // SMS afsendending er gået beserk STOP system
  digitalWrite(LED_PIN, HIGH);
  powerDown();
  do {
  } 
  while (1);
}

void powerUpSIM900() {
  Serial.println("Sim900 initiated");
  SIM900.begin(19200); // for GSM shield
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(2000);
  digitalWrite(9, LOW);
  delay(3000);

  SIM900.print("AT+IPR=19200\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);

  // SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  // delay(100);
  do {
    String ready = readGSM();
    if (ready.indexOf("RDY")) {
      Serial.println("Network OK");
      break;
    }  
    Serial.print(ready);
    delay(1);
  }
  while (1);

  Serial.println("Sim900 Ready...\n");
  isSIM900Running = true;
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
}

void boot() {
  Serial.println("SIM900 bootes");
  powerDown();
  delay(7000);
  powerUpSIM900();
}

void powerDown() {
  SIM900.print("AT+CPOWD=1\r");
  delay(100);
  isSIM900Running = false;
  Serial.println("Power down sendt til SIM900");
}

String readGSM() {
  char i_char = 0;
  String text;
  while (SIM900.available() > 0) {
    i_char = SIM900.read(); 
    if (i_char != 10 && i_char != 13) {
      text += i_char;
    } 
    delay(1);
  }
  return text;
}

String gemAfsenderTlf(String text) {
  Serial.print(" fra ");

  int pos = text.indexOf('+', 5);
  String tlf = text.substring(pos, pos + 11);
  Serial.println(tlf);
  return tlf;  
}

String gemAfsenderTS(String text) {

  int pos = text.indexOf('/');
  String timeStamp = text.substring(pos-2, pos + 15);
  String dkDate = timeStamp.substring(6,8) + "." + timeStamp.substring(3,5) + "." +  timeStamp.substring(0,2) + " " + timeStamp.substring(9,17);
  Serial.print(dkDate);
  return dkDate; 
}

long antalBeskeder = 0;
void sendSMS(String tlf, String besked) {
  if (antalBeskeder++ > 50) {
    haltSystem();
  }

  SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
  delay(100);
  String atTlf = "AT + CMGS = \"" + tlf + "\"";
  SIM900.println(atTlf);
  delay(100);

  SIM900.println(besked);
  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000); // give module time to send SMS
  SIM900.println("AT+CMGD=1,4"); // delete all SMS

}

void teleOperator(String tlf, String text) {
  int pos = text.indexOf("\"",19);
  Serial.print("Operator: ");
  Serial.println(text.substring(pos + 1, text.length() - 3));
  //  Serial.println(text);
  String sms = "Operator: " + text.substring(pos + 1, text.length() - 3);
  // sendSMS(tlf, sms);
}

void strength(String tlf, String text) {
  int pos = text.indexOf(":");
  Serial.print("Signal Strength: ");
  Serial.println(text.substring(pos + 2, text.length() - 4));

  //sendSMS(tlf, "Signal Strength: " + text.substring(pos + 2, text.length() - 4));
}

void timeStamp(String tlf, String text) {
  int pos = text.indexOf("\"");
  String tS= text.substring(pos + 1, text.length() - 6);
  String dkTS = tS.substring(6,8) + "." + tS.substring(3,5) + ".20" +  tS.substring(0,2) + " " + tS.substring(9,17);
  Serial.print("TimeStamp: ");
  Serial.println(dkTS);
  syncTime(tS);

  //sendSMS(tlf, "TimeStamp: " + dkTS);
}

void syncTime(String tS) {
  String tt = tS.substring(9,11);
  String mm = tS.substring(12,14);
  String ss = tS.substring(15,17);
  String dd = tS.substring(6,8);
  String mo = tS.substring(3,5);
  String aa = tS.substring(0,2);
  char tmp[5];
  tt.toCharArray(tmp, sizeof(tmp));
  int itt = atoi(tmp);
  mm.toCharArray(tmp, sizeof(tmp));
  int imm = atoi(tmp);
  ss.toCharArray(tmp, sizeof(tmp));
  int iss = atoi(tmp);
  dd.toCharArray(tmp, sizeof(tmp));
  int idd = atoi(tmp);
  mo.toCharArray(tmp, sizeof(tmp));
  int imo = atoi(tmp);
  aa.toCharArray(tmp, sizeof(tmp));
  int iaa = atoi(tmp);
  setTime(itt,imm,iss,idd,imo,iaa);
  //  Serial.println("Klokken er synkroniseret");
}




