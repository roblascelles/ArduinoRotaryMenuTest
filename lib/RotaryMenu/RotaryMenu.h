#ifndef RotaryMenu_H
#define RotaryMenu_H

#include <RotaryControl.h>
#include <LiquidCrystal_I2C.h>

class RotaryMenu
{
private:
  RotaryControl& _control;
  LiquidCrystal_I2C& _lcd;

  int _pos = 0;

  void turned(int diff);
  void pressed(ClickType pressType);

  static void turnedCallback(int diff, void* sender);
  static void pressedCallback(ClickType pressType, void* sender);

public:
  RotaryMenu(RotaryControl& control, LiquidCrystal_I2C& lcd);
};


#endif