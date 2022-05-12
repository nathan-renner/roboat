/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4BT.h>
#include <usbhub.h>
#include <Servo.h>

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo Servo1;
Servo Servo2;
Servo Servo3;

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

bool IS_MANUAL_CONTROL = true;
bool limit = false;
int xPos = 0;
int yPos = 0;
int isManual = 0;
int prevIsManual = isManual;
int x = 0;
int y = 0;
int angle = 0;

void setup() {
  Serial.begin(115200);
  ESC1.attach(31, 1000, 2000);
  ESC2.attach(33, 1000, 2000);
  ESC3.attach(35, 1000, 2000);
  Servo1.attach(37);
  Servo2.attach(39);
  Servo3.attach(41);

    // Serial.println("[Starting up...]");
  // Calibrate Servos to be at 90 (in the middle)
  Servo1.write(90);
  Servo2.write(90);
  Servo3.write(90);
  // Calibrate the ESCs by holding 0 for 5 seconds.
  ESC1.write(0);
  ESC2.write(0);
  ESC3.write(0);
  delay(5000);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}

void setSpeed(int speed) {
    speed = map(speed, 0, 100, 0, 160);
    ESC1.write(speed);
    ESC2.write(speed);
    ESC3.write(speed);
}

// Sets speed and a delay value "del" in milliseconds
void setSpeedWithDelay(int speed, int del) {
    setSpeed(speed);
    delay(del);
}

// 0: straight 
void turn(int angle) {
    angle = map(angle, -90, 90, 60, 120);
    Servo1.write(angle + 4); // resetting for calibration
    Servo2.write(angle + 5);
    Servo3.write(angle);
}

void turnWithDelay(int angle, int del) {
    turn(angle);
    delay(del);
}

void turnAndSetSpeedWithDelay(int angle, int speed, int del) {
    turn(angle);
    setSpeed(speed);
    delay(del);
}

void getPs4Data() {
  yPos = PS4.getAnalogHat(LeftHatY);
  if (yPos > 126) yPos = 126;
  xPos = PS4.getAnalogHat(RightHatX);
  if(limit) {
    y = map(yPos, 0, 126, 50, 0);
  } else {
    y = map(yPos, 0, 126, 100, 0);
  }
  
  angle = map(xPos, 0, 255, -90, 90);

  //angle = atan2(y, x) * 180 / PI;
  //angle = map(angle, 0, 180, 90, -90);
  if (PS4.getButtonClick(TRIANGLE)){
    IS_MANUAL_CONTROL = !IS_MANUAL_CONTROL;  
  }
    if (PS4.getButtonClick(SQUARE)){
    limit = !limit;  
  }
  
}
void loop() {
  Usb.Task();

  if (PS4.connected()) {

    getPs4Data();
    turnAndSetSpeedWithDelay(angle, y, 50);
    Serial.print("Y: ");
    Serial.println(y);
    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.println("=====<>=====");
  }
}
