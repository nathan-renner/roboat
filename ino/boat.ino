#include <BufferedInput.h>
#include <BufferedOutput.h>
#include <loopTimer.h>
#include <millisDelay.h>
#include <PinFlasher.h>
#include <SafeString.h>
#include <SafeStringNameSpace.h>
#include <SafeStringNameSpaceEnd.h>
#include <SafeStringNameSpaceStart.h>
#include <SafeStringReader.h>
#include <SafeStringStream.h>
#include <SerialComs.h>

#include <Servo.h>
#include <TinyGPSPlus.h>
#include "Waveshare_10Dof-D.h"


#define csf createSafeString 
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo Servo1;
Servo Servo2;
Servo Servo3;

//variables for voltage sensor



TinyGPSPlus gps;

bool IS_MANUAL_CONTROL = false;
char remoteInstruc[100];



csf(boatInstr, 20, "");
csf(dataForPi, 20, "");
char voltage[10];
double lat, lon, vel, yaw, dir;

int y, angle, speed;

csf(y_str, 3, "");
csf(angle_str, 3, "");
csf(funcCall, 3, "");
int num = 0;
//String piMessage = "";
csf(piMessage, 20, "");

void setup() {
 

    // Start the serial monitor on port 9600
    Serial.begin(9600);
    Serial2.begin(115200);
    //Serial2.begin(9600);
    Serial3.begin(9600);
    //pinMode(LED_BUILTIN, OUTPUT);

    // Attach all 3 ESCs
    ESC1.attach(46, 1000, 2000);
    ESC2.attach(48, 1000, 2000);
    ESC3.attach(50, 1000, 2000);
    Servo1.attach(47);
    Servo2.attach(49);
    Servo3.attach(51);
    
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
    dir = gps.course.deg();
    lat = gps.location.lat();
    lon = gps.location.lng();
    vel = gps.speed.kmph();
    
    // Serial.println("[Ready!]");
}


// Takes in a value "speed" from 0 to 100 (maps from 0 to 180)
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
    angle = map(angle, -90, 90, 65, 115);
    Servo1.write(angle + 4);
    Servo2.write(angle + 5);
    Servo3.write(angle + 5);
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

// Return GPS Coordinats and important IMU data (acceleration)
String getGPSData() {
    String dataForPi = "";
    lat = gps.location.lat();
    lon = gps.location.lng();
    vel = gps.speed.kmph();
    dir = gps.course.deg();
    dataForPi += String(lat, 7) + ";" + String(lon, 7) + ";" + String(vel, 7) + ";" + String(dir, 7);

    return dataForPi;
    
    
}


void sendPiMessage(String toSend) {
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      Serial.print(toSend);
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
    sendPiMessage(getGPSData());
    dataForPi = "";
  }
}
void loop() {

    // Resets values
    funcCall = y_str = angle_str = boatInstr = "";
    num = 0;
    

    //delay(50);
    readPiMessage();
    if (piMessage == "S") {
      IS_MANUAL_CONTROL = false;
      Serial.write('K');
    }
    
    
    
    if(Serial2.available() > 5) {
      while (1) {
        if (Serial2.read() == 'S') {
          break;
        }
      }
      //Serial2.readBytes(remoteInstruc, 40);
      Serial2.readBytesUntil("/", remoteInstruc, 20);

    } else {
      y = 0;
      angle = 0;
    }
      


    //Serial.println("Read radio instructions");
    
    

    // Tests if received correct command
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
    Serial.println(boatInstr);

    //If manual control is true it receives instruction from base station. Else, it receives instructions from the pi
    if (IS_MANUAL_CONTROL) {
      // Splits received command into function call and params
      // "[function name] [param 1] [param 2] [...param n]"
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

    }
    else {
      for (int i = 0; i < piMessage.length(); i++) {
         if(piMessage[i] == ";") {
          num++;
          i++;
        }
        if (piMessage[0] == "R") 
          break;

        
        else if(num == 0) {
          angle_str += piMessage[i];
        } else if(num == 1) {
            y_str += piMessage[i];
        } else {
          break;
        }       
      }
        //Sends sensor data to the pi
        parsePiMessage();      
    }

 

    // Converts param to int if state changed
    if (y_str != "")
      y_str.toInt(y);
    if (angle_str != "")
      angle_str.toInt(angle);


    
    
    

     


    turnAndSetSpeedWithDelay(angle, y, 50);


    
}
