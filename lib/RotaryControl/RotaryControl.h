#ifndef RotaryControl_H
#define RotaryControl_H

#include <Arduino.h>

enum ClickType { click_short, click_long };
enum ButtonState { button_down, button_up, button_down_long };

typedef void (*turnCallback)(int pos, void* sender);
typedef void (*pressCallback)(ClickType pressType, void* sender);

class RotaryControl
{
private:
  int _clkPin;
  int _dtPin;
  int _swPin;

  void* _sender;
  turnCallback _turnCallback; 
  pressCallback _pressCallback; 

  uint8_t _rotorState = 0;
  uint8_t _clk = 0;
  uint8_t _cclk = 0;

  uint8_t _buttonHistory =  0b11111111;
  ButtonState _buttonState = button_up;
  uint32_t _buttonDownMillis;
  
  void checkTurnedState();
  void checkButtonState();
  
  static void turnedInterupt();
  static void buttonInterupt();

public:
  RotaryControl(int clk, int dt, int sw);
  static void init();
  void setCallbacks(turnCallback turnCallback, pressCallback pressCallback, void* sender);
};

#endif