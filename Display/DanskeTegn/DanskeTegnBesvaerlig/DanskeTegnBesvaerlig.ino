#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
#define BACKLIGHT_PIN     13
 
 
//LiquidCrystal_I2C lcd(0x27);  // Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 
 
//LiquidCrystal_I2C lcd(0x38, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address
 
  
// Creat a set of new characters
//const uint8_t charBitmap[][8] = {
//   { B00110,
//  B00110,
//  B00000,
//  B01110,
//  B10001,
//  B10011,
//  B01101, },
//};

byte charBitmap[][8] = {
  { B00000,  B10001, B00000, B00000, B10001, B01110, B00000, },
  { B00000,  B10001, B00000, B00000, B10001, B01110, B00000, },
  { B00000,  B10001, B00000, B00000, B10001, B01110, B00000, },
  { B00000,  B10001, B00000, B00000, B10001, B01110, B00000, },
  { B00000,  B10001, B00000, B00000, B10001, B01110, B00000, },
  { B00000,  B10001, B00000, B00000, B10001, B01110, B00000, },
  { B00000,  B10001, B00000, B00000, B10001, B01110, B00000, },
  { B00000,  B11100, B11000, B11111, B00100, B00110, B00111, },
};
 

 
void setup()
{
   int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
 
  // Switch on the backlight
//  pinMode ( BACKLIGHT_PIN, OUTPUT );
//  digitalWrite ( BACKLIGHT_PIN, HIGH );
   lcd.backlight();
 
  lcd.begin(20,4);               // initialize the lcd
 
   for ( int i = 0; i < charBitmapSize; i++ )
   {
      lcd.createChar ( i, (uint8_t *)charBitmap[i] );
   }
 
 
  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO "); 
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" FORUM - fm   ");
  delay ( 1000 );
}
 
 
void loop()
{
   lcd.home ();
   // Do a little animation by writing to the same location
   for ( int i = 0; i <= 4; i++ )
   {
      for ( int j = 0; j < 20; j++ )
      {
//         lcd.print (char(random(8)));
         lcd.print (char(7));
      }
      lcd.setCursor ( 0, i );
   }
   delay (2000);
}
