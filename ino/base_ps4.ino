/*
 Example sketch for the PS4 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS4USB PS4(&Usb);

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
    String payload = man_ctrl + ";go;" + String(angle) + ";" + String(y) + ";/";
//    payload.toCharArray(packet, 100);
    Serial.print(payload);
//    Serial.write(packet, 100);
//    memset(packet, 0, 100);
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
  if (PS4.getButtonClick(TRIANGLE)){ // This turns manual control on or off
    IS_MANUAL_CONTROL = !IS_MANUAL_CONTROL;  
  }
    if (PS4.getButtonClick(SQUARE)){ // This limits the maximum speed to 50%
    limit = !limit;  
  }
  
}
void loop() {
  Usb.Task();

  if (PS4.connected()) {
    getPs4Data();
    sendPacket();
    delay(50);

  }
}
