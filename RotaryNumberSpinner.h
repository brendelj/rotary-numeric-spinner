#ifndef ROTARY_NUMBER_SPINNER_H
#define ROTARY_NUMBER_SPINNER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <EncoderButton.h>

#define MAX_LIST_SIZE 50

class RotaryNumberSpinner {
public:
    RotaryNumberSpinner(LiquidCrystal_I2C &lcd_, Encoder &encoder_, EncoderButton &button_);
    void startRange(const char* label, bool isFloat_, float start, float end, float step, void (*onSave)(float), void (*onCancel)());
    void startList(const char* label, bool isFloat_, const float *list, int size, void (*onSave)(float), void (*onCancel)());
    void update();
    void handleClick(EncoderButton &b);
    void handleLongPress(EncoderButton &b);
    bool isActive();
    void draw();

private:
    LiquidCrystal_I2C &lcd;
    Encoder &encoder;
    EncoderButton &button;
    bool active;
    bool useRange;
    bool isFloat;
    float rangeStart, rangeEnd, rangeStep;
    float numberList[MAX_LIST_SIZE];
    int listSize;
    int currentIndex;
    float currentValue;
    long lastEnc;
    const char* labelText;
    void (*saveCallback)(float);
    void (*cancelCallback)();
};

#endif
