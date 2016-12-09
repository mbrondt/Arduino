#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 7

const int ledPin =  13;      // the number of the LED pin
int ledState = LOW; // ledState used to set the LED


void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
  for (int i = 8; i < 12; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println("Setup finished");
  Serial.println();
}

int oldTempSerial = 0;
int oldTempRelays = 0;
long previousMillis = 0;
int lineWidth = 0;
long relayTime = 0;
int antalPrikLinier = 0;
void loop()
{
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > 1000) {
    previousMillis = currentMillis;
    initDHT11();
  //  oldTempSerial = dispSerial(oldTempSerial);
  }

  if ((currentMillis - relayTime > 10000)) {
    relayTime = currentMillis;
    oldTempRelays = dispRelays(oldTempRelays);
  }
  // delay(5000);
}

long dispSerial(long oldTemp) {
  int temp = (int)DHT11.temperature;
  if(oldTemp != temp) {
    oldTemp = temp;
    Serial.println();
    Serial.print("Temperature (C): ");
    Serial.print((float)DHT11.temperature);

    Serial.print(" - Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);
    //    Serial.print("----");
    lineWidth = 0;
  }
  else {

    if (lineWidth < 60) {
      lineWidth++;
      Serial.print(".");
    } 
    else{
      lineWidth = 0;
      antalPrikLinier++;
      Serial.print(". ");
      Serial.println(antalPrikLinier);
    }
  }
  return oldTemp;
}

long dispRelays(long oldTemp) {
  int temp = (int)DHT11.temperature;
  if(oldTemp != temp) {
    oldTemp = temp;

    for (int i = 8; i < 12; i++) {
      digitalWrite(i, HIGH);
    }
    digitalWrite(ledPin, LOW);

    const int offset = 25;

    switch (temp) {
    case offset + 1:
      digitalWrite(8, LOW);
      break;
    case offset + 2:
      digitalWrite(9, LOW);
      break;
    case offset + 3:
      digitalWrite(10, LOW);
      break;
    case offset + 4:
      digitalWrite(11, LOW);
      break;
    case offset + 5:
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      break;
    case offset + 6:
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case offset + 7:
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      break;
    case offset + 8:
      digitalWrite(8, LOW);
      digitalWrite(10, LOW);
      break;
    case offset + 9:
      digitalWrite(9, LOW);
      digitalWrite(11, LOW);
      break;
    case offset + 10:
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      break;
    case offset + 11:
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      break;
    default:
      digitalWrite(ledPin, HIGH);
      break;
    }
  }
  return oldTemp;
}

void initDHT11() {
  int chk = DHT11.read(DHT11PIN);

  switch (chk)
  {
    //  case DHTLIB_OK: 
    //    Serial.println("DHTLIB_OK");
    //    break;
  case DHTLIB_ERROR_CHECKSUM: 
  //  Serial.println("DHTLIB_ERROR_CHECKSUM");
    break;
  case DHTLIB_ERROR_TIMEOUT: 
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);
  delay(5000);
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);
  
  //  Serial.println("DHTLIB_ERROR_TIMEOUT");
    break;
  default: 
    break;
  }
}
























