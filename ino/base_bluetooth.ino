/*
 Example sketch for the PS4 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */


#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;


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
char packet[100] = "";
String man_ctrl;

void setup() {
  Serial.begin(9600);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 USB Library Started"));
}

void sendPacket() {
  if (IS_MANUAL_CONTROL){
    man_ctrl = "T";
  }
  else {
    man_ctrl = "F";
  }
    String payload = "S" + man_ctrl + ";go;" + String(angle) + ";" + String(y) + ";/";
//    payload.toCharArray(packet, 100);
    Serial.print(payload);
//    Serial.write(packet, 100);
//    memset(packet, 0, 100);
}

void getPs4Data() {
  if (PS4.getButtonClick(TRIANGLE)){ // This turns manual control on or off
    IS_MANUAL_CONTROL = !IS_MANUAL_CONTROL;  
  }
    if (PS4.getButtonClick(SQUARE)){ // This limits the maximum speed to 50%
    limit = !limit;  
  }
  yPos = PS4.getAnalogHat(LeftHatY);
  if (yPos > 126) yPos = 126;
  xPos = PS4.getAnalogHat(RightHatX);
  if(limit) {
    y = map(yPos, 0, 126, 50, 0);
  } else {
    y = map(yPos, 0, 126, 100, 0);
  }
  
  angle = map(xPos, 0, 255, -90, 90);

  
}


void loop() {
  Usb.Task();

  if (PS4.connected()) {
    getPs4Data();
    sendPacket();
    //This Sets the PS4 Led to indicate whether manual control is on or off and whether the max is velocity is set to 50%
    if (IS_MANUAL_CONTROL && limit) {
      PS4.setLed(Yellow);
    }
    else if(IS_MANUAL_CONTROL) {
      PS4.setLed(Blue);
    }
    else {
      PS4.setLed(Red);
    }
    
    delay(50);

  }
}
