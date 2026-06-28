#ifndef ROTARYKEYBOARD_H
#define ROTARYKEYBOARD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <EncoderButton.h>

class RotaryKeyboard {
public:
    RotaryKeyboard(LiquidCrystal_I2C &lcd_, Encoder &encoder_, EncoderButton &button_, const char* chars, byte backspaceCharIndex_, byte saveCharIndex_, byte cancelCharIndex_, byte arrowCharIndex_);
    void begin();
    void start(String prompt_, void (*onSave_)(String), void (*onCancel_)());
    void update();
    bool isActive();
    static void handleClick(EncoderButton &b);
    static void handleLongPress(EncoderButton &b);
    static void handleDoubleClick(EncoderButton &b);

private:
    LiquidCrystal_I2C &lcd;
    Encoder &encoder;
    EncoderButton &button;
    const char* characters;
    byte arrowCharIndex, backspaceIndex, saveIndex, cancelIndex;
    byte arrowChar[8], backspaceChar[8], saveChar[8], cancelChar[8];
    int charLength;
    int scrollPos = 0;
    int prevCenterCharIndex = -1;
    long lastEncCount = 0;
    bool active = false;
    String currentValue;
    String promptText;
    char prevChars[20];
    void initCustomChars();
    void updateDisplay();
    void printChar(char c);
    void handleChar(char selectedChar);
    void onClick(EncoderButton &eb);
    void onLongPress(EncoderButton &eb);
    void onDoubleClick(EncoderButton &eb);
    static RotaryKeyboard* instance;
};

#endif
