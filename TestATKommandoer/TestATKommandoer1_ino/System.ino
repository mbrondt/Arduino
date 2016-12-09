void haltSystem() {
  // SMS afsendending er gået beserk STOP system
  digitalWrite(LED_PIN, HIGH);
  do {
  } 
  while (1);
}

void powerUpSIM900() {
  Serial.println("Sim900 initiated");
  SIM900.begin(19200); // for GSM shield
  SIM900power();  // turn on shield
  delay(10000);  // give time to log on to network.

  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  //  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  //delay(100);
  Serial.println("Sim900 Ready...\n");
  isSIM900Running = true;
}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}

