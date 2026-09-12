#include "saptbsPID.h"
#include <Servo.h>
#include <Wire.h>
#include <AS5600.h>
#include <LiquidCrystal.h>


PIDsetup motor;
const uint8_t escPin = 6;

const uint8_t rs = 7;
const uint8_t en = 8;
const uint8_t d4 = 9;
const uint8_t d5 = 10;
const uint8_t d6 = 11;
const uint8_t d7 = 12;

const uint8_t xPin = A0;
const uint8_t yPin = A1;
const uint8_t jButton = 2;
int xVal;
int yVal;

int allow = 1;
int prev_allow = -600;

float kp = 6.f;
float kd = 0.6f;
float ki = 1.f;

float liveAngle = 0.0f;

float prev_kp = -600;
float prev_ki = -600;
float prev_kd = -600;
int prev_target = -600;

int i = 0;
int prev_i = -1;

String menus[5][2] = {{"Live Degree" , "Degree"},

                      {"Parameters", "Kp"},

                      {"Parameters", "Ki"}, 

                      {"Parameters", "Kd"}, 

                      {"Target", "Degree"}
};


byte upDownArrow[8] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000
};

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float target = 654.0f * (360./4095.);

AS5600 as5600;
unsigned long lasTime = 0;
unsigned long lastLCDTime = 0;
const int inputDelay = 250;


Servo esc;

void setup(){
  Wire.begin();
  as5600.begin();

  lcd.begin(16, 2);
  lcd.createChar(1, upDownArrow);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(jButton, INPUT_PULLUP);

 Serial.begin(9600);

  esc.attach(escPin, 1000, 2000);
  pid_setup(&motor, kp, ki, kd,  2000, 1000);

  esc.writeMicroseconds(1000);
  delay(2000);

}

void loop(){

  lcdControl();
  
  unsigned long currentTime = millis();
  float dt = (currentTime - lasTime) / 1000.0f;

  if(dt >= 0.02f){
    lasTime = currentTime;

    float checkAngle = as5600.rawAngle();
    float wrongAngle = checkAngle + 1000;

      if (wrongAngle >= 4096) {
    wrongAngle -= 4096;
  } else if (wrongAngle < 0) {
    wrongAngle += 4096;
  }

    wrongAngle *=  (360./4095.);
    liveAngle = wrongAngle;
    
    float output = pid_calculate(&motor, target, wrongAngle, dt);
    esc.writeMicroseconds((int)output);
    Serial.println(output);
  }
}

void lcdControl(){
    unsigned long currentMillis = millis();
  
  xVal = analogRead(xPin);
  yVal = analogRead(yPin);

  if (currentMillis - lastLCDTime >= inputDelay){
  
    if(digitalRead(jButton) == LOW){
      allow*=(-1);
      lastLCDTime = currentMillis;
    }
  
  
    if(allow>0){
      if(xVal>800){
        if(i<4){
          i++;
          lastLCDTime = currentMillis;
        }else{
          i=0;
          lastLCDTime = currentMillis;
        }
      }else if(xVal<200){
        if(i>0){
          i--;
          lastLCDTime = currentMillis;
        }else{
          i=4;
          lastLCDTime = currentMillis;
        }
      }
    }
  
    if(allow<0){
      if(yVal < 200){
        if(i==1) motor.kp += 0.1;
        else if(i == 2) motor.ki +=0.1;
        else if(i==3) motor.kd += 0.1;
        else if(i==4) target -= 5;
        lastLCDTime = currentMillis;
      }else if(yVal > 800){
        if(i==1) motor.kp -= 0.1;
        else if(i == 2) motor.ki -=0.1;
        else if(i==3) motor.kd -= 0.1;
        else if(i==4) target += 5;
        lastLCDTime = currentMillis;
      }
    }

  }
  if(i!=prev_i || motor.kp!=prev_kp || motor.ki!=prev_ki || motor.kd!=prev_kd || target!=prev_target || allow != prev_allow){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(menus[i][0]);



    lcd.setCursor(0, 1);
    lcd.print(menus[i][1]);
    lcd.print(": ");
    if(allow<0){
      if(i>0) lcd.write(byte(1));
    }



    if(i==0) lcd.print((-1)*(liveAngle - (654.0f * (360./4095.))));
    else if(i==1) lcd.print(motor.kp, 1);
    else if(i==2) lcd.print(motor.ki, 1);
    else if(i==3) lcd.print(motor.kd, 1);
    else if(i==4) lcd.print((-1)*(target - (654.0f * (360./4095.))));


    

    prev_i = i;
    prev_kp = motor.kp;
    prev_ki = motor.ki;
    prev_kd = motor.kd;
    prev_target = target;
    prev_allow = allow;
  }
}
