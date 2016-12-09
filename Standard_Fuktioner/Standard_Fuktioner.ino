void setup()
{
  pinMode(LED_PIN, OUTPUT); 
  Serial.begin(19200);
  powerUpSIM900(); 
}

void haltSystem() {
  // SMS afsendending er gået beserk STOP system
  digitalWrite(LED_PIN, HIGH);
  powerDown();
  do {
  } 
  while (1);
}

boolean isSIM900Running = false;
void powerUpSIM900() {
  Serial.println("Sim900 initiated");
  SIM900.begin(19200); // for GSM shield

  SIM900.print("AT\r"); //Er der liv
  delay(100);
  if (SIM900.available() == 0) {
    Serial.println("Koldstart af SIM900");
    digitalWrite(9, LOW);
    delay(1000);
    digitalWrite(9, HIGH);
    delay(2000);
    digitalWrite(9, LOW);
    delay(3000);
  } else {
    Serial.println("Varmstart af SIM900");
  }    

  SIM900.print("AT+IPR=19200\r");  // set SMS mode to text
  delay(100);
  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);

  // SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  // delay(100);

  Serial.println("Power up finished\n");
  isSIM900Running = true;
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

