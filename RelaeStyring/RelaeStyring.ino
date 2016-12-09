#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 7

const int ledPin =  13;      // the number of the LED pin
int ledState = LOW;             // ledState used to set the LED

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
  for (int i = 7; i < 11; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println("Setup finished");
  Serial.println();
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);

  switch (chk)
  {
    //  case DHTLIB_OK: 
    //    Serial.println("DHTLIB_OK");
    //    break;
  case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("DHTLIB_ERROR_CHECKSUM");
    break;
  case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("DHTLIB_ERROR_TIMEOUT");
    break;
  default: 
    break;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((int)DHT11.temperature);

  if ((int)DHT11.temperature >= 26) {
    Serial.println("Over");
    digitalWrite(ledPin, HIGH);
    for (int i = 7; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  } 
  else {
    Serial.println("Under");
    digitalWrite(ledPin, LOW);
    for (int i = 10; i > 6; i--) {
      digitalWrite(i, HIGH);
    }  
  }

  /*  digitalWrite(ledPin, HIGH);
   for (int i = 7; i < 11; i++) {
   digitalWrite(i, LOW);
   delay(20);
   }
   delay(1000);
   digitalWrite(ledPin, LOW);
   for (int i = 10; i > 6; i--) {
   digitalWrite(i, HIGH);
   delay(20);
   }
   */
  delay(1000);

  //  delay(5000);
}


