#include "RotaryNumberSpinner.h"

RotaryNumberSpinner::RotaryNumberSpinner(LiquidCrystal_I2C &lcd_, Encoder &encoder_, EncoderButton &button_)
    : lcd(lcd_), encoder(encoder_), button(button_), active(false), useRange(false), isFloat(false), rangeStart(0), rangeEnd(0), rangeStep(1), listSize(0), currentIndex(0), currentValue(0), lastEnc(0), saveCallback(nullptr), cancelCallback(nullptr) {}

void RotaryNumberSpinner::startRange(const char* label, bool isFloat_, float start, float end, float step, void (*onSave)(float), void (*onCancel)()) {
    labelText = label;
    useRange = true;
    isFloat = isFloat_;
    rangeStart = start;
    rangeEnd = end;
    rangeStep = step;
    saveCallback = onSave;
    cancelCallback = onCancel;
    currentValue = rangeStart;
    lastEnc = encoder.read();
    active = true;
    draw();
}

void RotaryNumberSpinner::startList(const char* label, bool isFloat_, const float *list, int size, void (*onSave)(float), void (*onCancel)()) {
    labelText = label;
    useRange = false;
    isFloat = isFloat_;
    listSize = (size > MAX_LIST_SIZE ? MAX_LIST_SIZE : size);
    for (int i = 0; i < listSize; i++) numberList[i] = list[i];
    saveCallback = onSave;
    cancelCallback = onCancel;
    currentIndex = 0;
    currentValue = numberList[currentIndex];
    lastEnc = encoder.read();
    active = true;
    draw();
}

void RotaryNumberSpinner::update() {
    if (!active) return;

    long encCount = encoder.read();
    long delta = encCount - lastEnc;
    if (abs(delta) >= 4) {
        int step = delta / 4;
        lastEnc += step * 4;
        step = -step;

        if (useRange) {
            float newVal = currentValue + (rangeStep * step);
            if (newVal < rangeStart) newVal = rangeStart;
            if (newVal > rangeEnd) newVal = rangeEnd;
            currentValue = newVal;
        } else {
            currentIndex += step;
            if (currentIndex < 0) currentIndex = 0;
            if (currentIndex >= listSize) currentIndex = listSize - 1;
            currentValue = numberList[currentIndex];
        }
        draw();
    }
}

void RotaryNumberSpinner::handleClick(EncoderButton &b) {
    if (!active) return;
    if (saveCallback) saveCallback(currentValue);
    active = false;
}

void RotaryNumberSpinner::handleLongPress(EncoderButton &b) {
    if (!active) return;
    if (cancelCallback) cancelCallback();
    active = false;
}

bool RotaryNumberSpinner::isActive() {
    return active;
}

void RotaryNumberSpinner::draw() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(labelText);

    int row = 2;

    lcd.setCursor(0, row);
    if ((useRange && currentValue > rangeStart) || (!useRange && currentIndex > 0)) {
        lcd.print("<");
    } else {
        lcd.print(" ");
    }

    lcd.setCursor(4, row);
    if (isFloat) {
        lcd.print(currentValue, 1);
    } else {
        lcd.print((int)currentValue);
    }

    lcd.setCursor(15, row);
    if ((useRange && currentValue < rangeEnd) || (!useRange && currentIndex < listSize - 1)) {
        lcd.print(">");
    } else {
        lcd.print(" ");
    }
}
