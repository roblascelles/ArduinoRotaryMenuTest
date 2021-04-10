#include <Arduino.h>
#include <RotaryControl.h>
#include <RotaryMenu.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int encoderSW = 6;
int encoderDT = 7;
int encoderCLK = 8;

RotaryControl control(encoderCLK, encoderDT, encoderSW);
LiquidCrystal_I2C lcd(0x27, 20, 4);

RotaryMenu menu(control, lcd);

void setup()
{
  Serial.begin(9600);

  while (!Serial)
  {
    ;
  }

  Serial.println("Start setup");

  lcd.init();
  lcd.backlight();
  lcd.clear();

  RotaryControl::init();
  
  Serial.println("End setup");
}

void loop()
{
  delay(1);
}
