#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <EncoderButton.h>
#include "RotaryKeyboard.h"
#include "RotaryNumberSpinner.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
Encoder encoder(A0, A1);
EncoderButton button(A2);

const char kbChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,!?:;+-*/=_@#";
RotaryKeyboard rk(lcd, encoder, button, kbChars, 1,2,3,4);
RotaryNumberSpinner rns(lcd, encoder, button);

String P_SID = "";
String P_USERNAME = "";
String P_PASSWORD = "";
float P_TARGET_TEMP = 175;
float P_FAN_SPEED = 50;
float P_HEAT_TEMP = 200;
float P_COOL_TEMP = 350;

const char* menuItems[] = {"Start Controller","Stop Controller","Set Target Temp","Set Heat Temp","Set Cool Temp","Set Fan Speed","Set NET SID","Set USERNAME","Set PASSWORD"};
const int menuLength = 9;
int menuIndex = 0;
int menuTopIndex = 0;
long lastEncCount = 0;
bool menuActive = false;

byte rkRightArrow[8] = {B00000,B11000,B01100,B00110,B11111,B00110,B01100,B11000};
byte rnsLeftArrow[8]  = {B00000,B00100,B00110,B00111,B00110,B00100,B00000,B00000};
byte rnsRightArrow[8] = {B00000,B00100,B01100,B11100,B01100,B00100,B00000,B00000};

void startController() { lcd.clear(); lcd.setCursor(0,1); lcd.print("Controller Started!"); Serial.println("Controller Started!"); }
void stopController() { lcd.clear(); lcd.setCursor(0,1); lcd.print("Controller Stopped!"); Serial.println("Controller Stopped!"); }

void onSaveSID(String value){ P_SID=value; menuActive=true; drawMenu(); }
void onSaveUsername(String value){ P_USERNAME=value; menuActive=true; drawMenu(); }
void onSavePassword(String value){ P_PASSWORD=value; menuActive=true; drawMenu(); }
void onSaveTargetTemp(float value){ P_TARGET_TEMP=value; menuActive=true; drawMenu(); }
void onSaveHeatTemp(float value){ P_HEAT_TEMP=value; menuActive=true; drawMenu(); }
void onSaveCoolTemp(float value){ P_COOL_TEMP=value; menuActive=true; drawMenu(); }
void onSaveFanSpeed(float value){ P_FAN_SPEED=value; menuActive=true; drawMenu(); }
void onCancel(){ lcd.setCursor(0,3); lcd.print("Cancelled"); menuActive=true; drawMenu(); }
void onSpinnerCancel(){ menuActive=true; drawMenu(); }

void drawMenu(){
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Use encoder & Btn");
    for(int row=0; row<3; row++){
        int idx = menuTopIndex+row;
        if(idx>=menuLength) break;
        lcd.setCursor(0,row+1); lcd.print("                    ");
        lcd.setCursor(0,row+1); lcd.write(idx==menuIndex?byte(0):' '); lcd.print(menuItems[idx]);
    }
}

void menuSelect(EncoderButton &b){
    if(!menuActive) return;
    menuActive = false;
    switch(menuIndex){
        case 0: startController(); lcd.clear(); break;
        case 1: stopController(); lcd.clear(); break;
        case 2: rns.startRange("Set Target Temp:", true, 175.0, 425.0, 25.0, onSaveTargetTemp, onSpinnerCancel); break;
        case 3: rns.startRange("Set Heat Temp:", true, 175.0, 425.0, 25.0, onSaveHeatTemp, onSpinnerCancel); break;
        case 4: rns.startRange("Set Cool Temp:", true, 50.0, 425.0, 25.0, onSaveCoolTemp, onSpinnerCancel); break;
        case 5: { float fanSpeeds[] = {0,25,50,75,100}; rns.startList("Set Fan Speed:", false, fanSpeeds, 5, onSaveFanSpeed, onSpinnerCancel); break; }
        case 6: rk.start("Set NET SID:", onSaveSID, onCancel); break;
        case 7: rk.start("Set USERNAME:", onSaveUsername, onCancel); break;
        case 8: rk.start("Set PASSWORD:", onSavePassword, onCancel); break;
    }
}

void setup() {
    Serial.begin(115200);
    lcd.init(); lcd.backlight(); rk.begin();
    lcd.createChar(0,rkRightArrow); lcd.createChar(1,rnsLeftArrow); lcd.createChar(2,rnsRightArrow);
    button.setClickHandler([](EncoderButton &b){ if(rk.isActive()) RotaryKeyboard::handleClick(b); else if(rns.isActive()){ rns.handleClick(b); menuActive=true; drawMenu(); } else if(menuActive) menuSelect(b); });
    button.setLongPressHandler([](EncoderButton &b){ if(rk.isActive()) RotaryKeyboard::handleLongPress(b); else if(rns.isActive()){ rns.handleLongPress(b); menuActive=true; drawMenu(); } else if(!menuActive){ menuActive=true; drawMenu(); } });
    button.setDoubleClickHandler([](EncoderButton &b){ if(rk.isActive()) RotaryKeyboard::handleDoubleClick(b); });
}

void loop(){
    rk.update(); rns.update(); button.update();
    if(menuActive && !rk.isActive() && !rns.isActive()){
        long encCount = encoder.read(); long delta = encCount - lastEncCount;
        if(abs(delta)>=4){
            int step = delta/4; lastEncCount+=step*4; menuIndex += step;
            if(menuIndex<0) menuIndex=0; if(menuIndex>=menuLength) menuIndex=menuLength-1;
            if(menuIndex<menuTopIndex) menuTopIndex=menuIndex; else if(menuIndex>=menuTopIndex+3) menuTopIndex=menuIndex-2;
            drawMenu();
        }
    }
}
