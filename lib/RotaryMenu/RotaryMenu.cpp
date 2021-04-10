#include <RotaryMenu.h>

#include <RotaryControl.h>
#include <LiquidCrystal_I2C.h>

RotaryMenu::RotaryMenu(RotaryControl& control, LiquidCrystal_I2C& lcd) : _control{control}, _lcd{lcd}
{
    _control.setCallbacks(turnedCallback, pressedCallback, this);
}

void RotaryMenu::turnedCallback(int diff, void* sender) {
    ((RotaryMenu *) sender)->turned(diff);
}

void RotaryMenu::pressedCallback(ClickType pressType, void* sender) {
    ((RotaryMenu *) sender)->pressed(pressType);
}

void RotaryMenu::turned(int diff)
{
    _pos += diff;

    _lcd.setCursor(0, 0);
    _lcd.print("Turned ");
    _lcd.println(_pos);
}

void RotaryMenu::pressed(ClickType pressType)
{
    _lcd.setCursor(0, 1);
    _lcd.print("Pressed ");
    _lcd.println(pressType);
}