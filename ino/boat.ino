#include <Servo.h>
//#include <Arduino_MKRGPS.h>

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo Servo1;
Servo Servo2;
Servo Servo3;

bool IS_MANUAL_CONTROL = false;
char remoteInstruc[100];
String boatInstr = "";
char gpsCoords[100];
double lat, lon, boatAngle, velocity;

int y, angle, speed;
String y_str, angle_str;
String funcCall = "";
int num = 0;
String piMessage = "";

void setup() {
    // Start the serial monitor on port 9600
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    // Attach all 3 ESCs
    ESC1.attach(8, 1000, 2000);
    ESC2.attach(9, 1000, 2000);
    ESC3.attach(10, 1000, 2000);
    Servo1.attach(11);
    Servo2.attach(12);
    Servo3.attach(13);
    
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

    //Initialize GPS coordinates
//    lat = GPS.latitude();
//    lon = GPS.longitude();
    
    // Serial.println("[Ready!]");
}

// Takes in a value "speed" from 0 to 100 (maps from 0 to 180)
void setSpeed(int speed) {
    speed = map(speed, 0, 100, 0, 160);
    ESC1.write(speed);
    ESC2.write(speed);
    ESC3.write(speed);
}

// 0: straight 
void turn(int angle) {
    angle = map(angle, -90, 90, 0, 180);
    Servo1.write(angle);
    Servo2.write(angle);
    Servo3.write(angle);
}

void turnAndSetSpeedWithDelay(int angle, int speed, int del) {
    turn(angle);
    setSpeed(speed);
    delay(del);
}

// Return GPS Coordinats and important IMU data (acceleration)
//void getGPSData() {
//    lat = GPS.latitude();
//    lon = GPS.longitude();
//}

// void sendPacket() {
//     String payload = String(lat) + " " + String(lon);
//     payload.toCharArray(gpsCoords, 100);
//     Serial1.write(gpsCoords, 100);
//     memset(gpsCoords, 0, 100);
// }

void verifyRadioCommand() {
  if(remoteInstruc[0] == 'F' || remoteInstruc[0] == 'T') {
      if (remoteInstruc[0] == 'T')
        IS_MANUAL_CONTROL = true;
      else
        IS_MANUAL_CONTROL = false;
        
      for(int i = 2; i<100; i++) {
        if(remoteInstruc[i]== '/') {
          break;
        }
        boatInstr += remoteInstruc[i];
      }
    }
}

void parseRadioCommand() {
  for (int i=0; i<boatInstr.length(); i++) {
    if(boatInstr[i] == ';') {
      num++;
      i++;
    }
    //Serial.println(num);
    if(num == 0) {
      funcCall += boatInstr[i];
    } else if(num == 1){
        angle_str += boatInstr[i];
    } else if(num == 2) {
        y_str += boatInstr[i];
    } else {
      break;
    }
  }

  // Converts param to int if state changed
  if (y_str != "")
    y = y_str.toInt();
  if (angle_str != "")
    angle = angle_str.toInt();
}

void displayCurrentValues() {
  Serial.println("=====<>=====");
  Serial.print("Manual Control: ");
  Serial.println(IS_MANUAL_CONTROL);
  Serial.println("Function: " + funcCall);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("Angle: ");
  Serial.println(angle);
  Serial.println("=====<>=====");
}

void sendPiMessage(string toSend) {
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      Serial.write(toSend);
    }
  }
}

void readPiMessage() {
  piMessage = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      piMessage += (char)Serial.read();
    }
    Serial.flush();
  }
}

void parsePiMessage() {
  if (piMessage == "R") {
    string toSend = (setPrecision(7) << lat) + ";" + (setPrecision(7) << lon) + ";" + (setPrecision(7) << boatAngle) + ";" + (setPrecision(7) << velocity);
  }
}

void loop() {
    // Resets values
    funcCall = y_str = angle_str = boatInstr = "";
    num = 0;

    Serial.readBytes(remoteInstruc, 100);

    // Tests if received correct command
    verifyRadioCommand();
    
    // if true, listen to radio
    // if false, listen to pi
    if (IS_MANUAL_CONTROL == true) {
      // Splits received command into function call and params
      parseRadioCommand();
      // Displays current values
      displayCurrentValues();

      turnAndSetSpeedWithDelay(angle, y, 50);
    } else {
      // send gps coords and acceleration to pi
      
      // receive go instruction with speed, angle, and delay
      readPiMessage();
      parsePiMessage();
      sendPiMessage();

      // turnAndSetSpeedWithDelay(angle, y, delay)
    }
}
