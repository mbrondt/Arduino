//Side 2
void haltSystem() {
  // SMS afsendending er gået beserk STOP system
  digitalWrite(LED_PIN, HIGH);
  do {
  } 
  while (1);
}

String setHeat(String beskedDel) {
  // Heat Command
  if (beskedDel.startsWith("*heat*")) {
    int pos = beskedDel.indexOf("*", 4);
    if (beskedDel.substring(pos+1) == "on#") {
      digitalWrite(RELAY_PIN, LOW);
    } 
    if (beskedDel.substring(pos+1) == "off#") {
      digitalWrite(RELAY_PIN, HIGH);
    } 

    if (!digitalRead(RELAY_PIN)) {
      return("Heat is on");
    } 
    else {
      return("Heat is off");
    }
  } // End of Heat
}

void skrivAfsender(String text) {
  Serial.println("Besked: ");

  int pos = text.indexOf('/');
  String timeStamp = text.substring(pos-2, pos + 15);
  String dkDate = timeStamp.substring(6,8) + "." + timeStamp.substring(3,5) + "." +  timeStamp.substring(0,2) + " " + timeStamp.substring(9,17);
  Serial.print(dkDate);

  Serial.print(" fra ");

  pos = text.indexOf('+', 5);
  String tlf = text.substring(pos, pos + 11);
  Serial.println(tlf);
}

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

int lowTemp = 100;
int highTemp = -100;
int lowHum = 100;
int highHum = -100;

String getDHT22Data(String type) {
  //Serial.print("DHT22, \t");
  int chk = DHT.read22(DHT22_PIN);
  switch (chk)
  {
    //  case DHTLIB_OK:  
    //    Serial.print("OK,\t"); 
    //    break;
  case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
  case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    //  default: 
    //    Serial.print("Unknown error,\t"); 
    //    break;
  }
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;

  if (temperature <= lowTemp) lowTemp = temperature;
  if (temperature >= highTemp) highTemp = temperature;
  if (humidity <= lowHum) lowHum = humidity;
  if (humidity >= highHum) highHum = humidity;

  delay(100);
  String retur; 
  if (type == "*temperature#") {
    retur = "Temperature: " + doubleToString(temperature,1) + "\nLow/High: " + lowTemp + "/" + highTemp;
  } 
  else {  
    retur = "Humidity: " + doubleToString(humidity,1) + "\nLow/High: " + lowHum + "/" + highHum;
  }
  //Serial.println(retur);
  return retur;
}  

long antalBeskeder = 0;

void sendSMS(String tlf, String timeStamp, String besked) {
  if (antalBeskeder++ > 50) {
    haltSystem();
  }

  SIM900.print("AT+CMGF=1\r"); // AT command to send SMS message
  delay(100);
  String atTlf = "AT + CMGS = \"" + tlf + "\"";
  SIM900.println(atTlf);
  delay(100);
  String svar = timeStamp + "\n" + besked + "\nmessage: " + antalBeskeder;
  SIM900.println(svar);
  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000); // give module time to send SMS
  SIM900.println("AT+CMGD=1,4"); // delete all SMS

}

void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
}




