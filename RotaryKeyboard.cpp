#include "RotaryKeyboard.h"
#include <string.h>

RotaryKeyboard* RotaryKeyboard::instance = nullptr;

void RotaryKeyboard::handleClick(EncoderButton &b) {
    if(instance) instance->onClick(b);
}
void RotaryKeyboard::handleLongPress(EncoderButton &b) {
    if(instance) instance->onLongPress(b);
}
void RotaryKeyboard::handleDoubleClick(EncoderButton &b) {
    if(instance) instance->onDoubleClick(b);
}

RotaryKeyboard::RotaryKeyboard(LiquidCrystal_I2C &lcd_, Encoder &encoder_, EncoderButton &button_, const char* chars, byte backspaceCharIndex_, byte saveCharIndex_, byte cancelCharIndex_, byte arrowCharIndex_)
    : lcd(lcd_), encoder(encoder_), button(button_), characters(chars), arrowCharIndex(arrowCharIndex_), backspaceIndex(backspaceCharIndex_), saveIndex(saveCharIndex_), cancelIndex(cancelCharIndex_) {
    charLength = strlen(characters);
    instance = this;
}

void RotaryKeyboard::initCustomChars() {
    byte defaultArrow[8] = {B00100,B01110,B11111,B00100,B00100,B00100,B00000,B00000};
    byte defaultBackspace[8] = {B00011,B00110,B01101,B11010,B01101,B00110,B00011,B00000};
    byte defaultSave[8] = {B00000,B00000,B00000,B00000,B00001,B01001,B11111,B01000};
    byte defaultCancel[8] = {B11011,B01010,B01010,B00100,B01010,B01010,B11011,B00000};
    memcpy(arrowChar, defaultArrow, 8);
    memcpy(backspaceChar, defaultBackspace, 8);
    memcpy(saveChar, defaultSave, 8);
    memcpy(cancelChar, defaultCancel, 8);
    lcd.createChar(arrowCharIndex, arrowChar);
    lcd.createChar(backspaceIndex, backspaceChar);
    lcd.createChar(saveIndex, saveChar);
    lcd.createChar(cancelIndex, cancelChar);
}

void RotaryKeyboard::begin() {
    initCustomChars();
    button.setClickHandler(handleClick);
    button.setLongPressHandler(handleLongPress);
    button.setDoubleClickHandler(handleDoubleClick);
    for(int i=0;i<20;i++) prevChars[i]='\0';
}

void RotaryKeyboard::start(String prompt_, void (*onSave_)(String), void (*onCancel_)()) {
    promptText = prompt_;
    currentValue = "";
    scrollPos = 0;
    lastEncCount = encoder.read();
    active = true;
    prevCenterCharIndex = -1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(promptText);
    for(int i=1;i<4;i++){
        lcd.setCursor(0,i);
        lcd.print("                    ");
    }
    updateDisplay();
}

bool RotaryKeyboard::isActive() { return active; }

void RotaryKeyboard::update() {
    if(!active) return;
    button.update();
    long encCount = encoder.read();
    long delta = encCount - lastEncCount;
    if(abs(delta) >= 4){
        int step = delta/4;
        scrollPos += step;
        lastEncCount += step*4;
        while(scrollPos < 0) scrollPos += charLength;
        while(scrollPos >= charLength) scrollPos -= charLength;
        updateDisplay();
    }
}

void RotaryKeyboard::updateDisplay() {
    int windowSize = 20;
    for(int i=0;i<windowSize;i++){
        int idx = (scrollPos - windowSize/2 + i + charLength) % charLength;
        char c = characters[idx];
        if(prevChars[i] != c){
            lcd.setCursor(i,1);
            printChar(c);
            prevChars[i] = c;
        }
    }
    lcd.setCursor(windowSize/2,2);
    lcd.write(byte(arrowCharIndex));
    lcd.setCursor(0,3);
    String displayWindow = (currentValue.length() <= 20) ? currentValue : currentValue.substring(currentValue.length() - 20);
    lcd.print(displayWindow);
    for(int i=displayWindow.length(); i<20; i++) lcd.print(" ");
}

void RotaryKeyboard::printChar(char c){
    switch(c){
        case '\n': lcd.write(byte(saveIndex)); break;
        case '\x1B': lcd.write(byte(cancelIndex)); break;
        case '\b': lcd.write(byte(backspaceIndex)); break;
        default: lcd.print(c);
    }
}

void RotaryKeyboard::handleChar(char selectedChar){
    if(selectedChar == '\n'){
        active=false;
        lcd.clear();
    } else if(selectedChar == '\x1B'){
        active=false;
        lcd.clear();
    } else if(selectedChar == '\b'){
        if(currentValue.length()>0) currentValue.remove(currentValue.length()-1);
    } else {
        currentValue += selectedChar;
    }
    if(active) updateDisplay();
}

void RotaryKeyboard::onClick(EncoderButton &eb) { handleChar(characters[scrollPos]); }
void RotaryKeyboard::onLongPress(EncoderButton &eb) { handleChar('\n'); }
void RotaryKeyboard::onDoubleClick(EncoderButton &eb) { handleChar('\x1B'); }
