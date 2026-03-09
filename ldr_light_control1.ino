
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LEDpin = 9;
const int LDRpin = A0;
const int potPin = A1;

int LEDstate = LOW;

void setup(){
  pinMode(LEDpin, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop(){
  int lightLevel = analogRead(LDRpin);
  int potVal = analogRead(potPin);

  int tolerance = 20;

  int onLevel = potVal - tolerance;
  int offLevel = potVal + tolerance;

  if (lightLevel < onLevel){
    LEDstate = HIGH;
  } 
  if (lightLevel > offLevel) {
    LEDstate = LOW;
  }
  digitalWrite(LEDpin, LEDstate);

  lcd.setCursor(0, 0);
  lcd.print("LDR:");
  lcd.print(lightLevel);
  lcd.print("  ");

  lcd.setCursor(0, 1);
  lcd.print("Set Level:");
  lcd.print(potVal);
  lcd.print("  ");
  delay(100);
  }