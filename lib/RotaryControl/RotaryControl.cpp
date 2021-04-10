#include <RotaryControl.h>

#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0

#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"

#define TIMER_INTERVAL_MS 100
#define HW_TIMER_INTERVAL_MS 50

#define BUTTON_HISTORY_MASK 0b11000011

static RotaryControl *controls[5];
static int controlCnt = 0;

SAMDTimer ITimer(TIMER_TC3);

RotaryControl::RotaryControl(int clk, int dt, int sw)
{
    _clkPin = clk;
    _dtPin = dt;
    _swPin = sw;

    pinMode(clk, INPUT);
    attachInterrupt(digitalPinToInterrupt(clk), RotaryControl::turnedInterupt, CHANGE);

    pinMode(dt, INPUT);
    attachInterrupt(digitalPinToInterrupt(dt), RotaryControl::turnedInterupt, CHANGE);

    pinMode(sw, INPUT_PULLUP);
    controls[controlCnt++] = this;
}

void RotaryControl::init()
{
    ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000L, RotaryControl::buttonInterupt);
}

void RotaryControl::setCallbacks(turnCallback turnCallback, pressCallback pressCallback, void* sender)
{
    _turnCallback = turnCallback;
    _pressCallback = pressCallback;
    _sender = sender;
}

void RotaryControl::turnedInterupt()
{
    for (int i = 0; i < controlCnt; i++)
    {
        controls[i]->checkTurnedState();
    }
}

void RotaryControl::buttonInterupt()
{
    for (int i = 0; i < controlCnt; i++)
    {
        controls[i]->checkButtonState();
    }
}

void RotaryControl::checkTurnedState()
{
    uint8_t state = digitalRead(_clkPin) * 2 + digitalRead(_dtPin);

    if ((state == 1 && _rotorState == 3) ||
        (state == 0 && _rotorState == 1) ||
        (state == 2 && _rotorState == 0) ||
        (state == 3 && _rotorState == 2))
    {
        _clk++;
    }
    else if ((state == 2 && _rotorState == 3) ||
             (state == 0 && _rotorState == 2) ||
             (state == 1 && _rotorState == 0) ||
             (state == 3 && _rotorState == 1))
    {
        _cclk++;
    }

    _rotorState = state;

    if (state == 3)
    {
        if (_clk > _cclk)
        {
            _turnCallback(+1, _sender);
        }
        else if (_clk < _cclk)
        {
            _turnCallback(-1, _sender);
        }

        _clk = _cclk = 0;
    }
}

void RotaryControl::checkButtonState()
{

    _buttonHistory = _buttonHistory << 1;
    _buttonHistory |= digitalRead(_swPin);

     uint32_t now = millis();

    if ((_buttonHistory & BUTTON_HISTORY_MASK) == 0b00000011)
    {
        _buttonHistory = 0b11111111;

        if (_buttonState == button_down) {
            _pressCallback(click_short, _sender);
        }

        _buttonState = button_up;
    }
    else if ((_buttonHistory & BUTTON_HISTORY_MASK) == 0b11000000)
    {
        _buttonHistory = 0b00000000;
        _buttonState = button_down;

        _buttonDownMillis = now;
    }

    if (_buttonState == button_down) {
        uint32_t downForMillis =  now - _buttonDownMillis;
        if (downForMillis > 250) {
            _buttonState = button_down_long;
            _pressCallback(click_long, _sender);
        }
    }

}
