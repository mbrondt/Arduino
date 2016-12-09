
#include <Servo.h>

Servo myServo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
//  Pis my
void setup() {
  myServo.attach(9); 
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Enter position to set Servo: 1 to 5");
  myServo.write(pos);
//  for (idx = 0; idx <=135; idx++){
//    myServo.write(pos);
//    delay(500);
//  }
//  for (idx = 135; idx >=0; idx--){
//    myServo.write(pos);
//    delay(500);
//  }
}

void loop() {
    if (Serial.available())
  {
    int idx = 0;
    char ch = Serial.read();
    if (ch >= '1' && ch <= '5')
    {
      int pos = ch - '0';
      myServo.write((pos-1) * 30);
      Serial.print("Servo sat til ");
      Serial.print(myServo.read());
      Serial.println(" gr.");
    }
    if (ch == 'x')
    {
      Serial.println("Cleared");
    }
  }

}

