String doubleToString(double input,int decimalPlaces){
  if(decimalPlaces!=0){
    String string = String((int)(input*pow(10,decimalPlaces)));
    if(abs(input)<1){
      if(input>0)
        string = "0"+string;
      else if(input<0)
        string = string.substring(0,1)+"0"+string.substring(1);
    }
    return string.substring(0,string.length()-decimalPlaces)+"."+string.substring(string.length()-decimalPlaces);
  }
  else {
    return String((int)input);
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT); 
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);

  Serial.begin(9600); // for serial monitor
  Serial.println("\nSketch initiated");
  SIM900Start();
}

void SIM900Start() {
  Serial.println("Sim900 initiated");
  SIM900.begin(19200); // for GSM shield
  SIM900toggle();
  delay(10000);  // give time to log on to network.

  SIM900.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  // SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  // delay(100);
  Serial.println("Sim900 Ready...\n");
  isSIM900Running = true;
}

void SIM900toggle()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}

void haltSystem() {
  // SMS afsendending er gået beserk STOP system
  digitalWrite(LED_PIN, HIGH);
  do {
  } 
  while (1);
}



