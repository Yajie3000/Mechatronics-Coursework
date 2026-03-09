#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo stampServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int potPin = A0;
int servoPin = 9;

int minAngle = 20;
int maxAngle = 160;

void setup()
{
  stampServo.attach(servoPin);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Stamp Machine");

  delay(2000);
  lcd.clear();
}

void loop()
{
  int potValue = analogRead(potPin);

  int range = map(potValue, 0, 1023, minAngle, maxAngle);

  lcd.setCursor(0,0);
  lcd.print("range: ");
  lcd.print(range);
  lcd.print("   ");

  // Move down
  for (int pos = range; pos >= minAngle; pos--) {
    stampServo.write(pos);
    delay(10);
  }

  delay(300);

  // Move back up
  for (int pos = minAngle; pos <= range; pos++) {
    stampServo.write(pos);
    delay(10);
  }

  delay(300);
}