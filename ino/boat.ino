#include <Servo.h>
#include <TinyGPSPlus.h>
#include "Waveshare_10Dof-D.h"


#define Led 3
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo Servo1;
Servo Servo2;
Servo Servo3;

//variables for voltage sensor

/*float adc_voltage = 0.0;
float in_voltage = 0.0;

float ref_voltage = 14.8;

int R1, R2;

int adc_value = 0;*/

TinyGPSPlus gps;

bool IS_MANUAL_CONTROL = false;
char remoteInstruc[100];
String dataForPi;
String boatInstr = "";
char voltage[10];
double lat, lon, vel, yaw, dir;

int y, angle, speed;
String y_str, angle_str;
String funcCall = "";
String readings = "";
int num = 0;
String piMessage = "";

void setup() {
    pinMode(Led, OUTPUT);

    // Start the serial monitor on port 9600
    Serial.begin(9600);
    Serial1.begin(115200);
    Serial2.begin(9600);
    Serial3.begin(9600);
    //pinMode(LED_BUILTIN, OUTPUT);

    // Attach all 3 ESCs
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
/*void readVoltage() {
  adc_value = analogRead(A0);

  adc_voltage = (adc_value*ref_voltage)/1024;

  in_voltage = adc_voltage/(R2/(R1+R2));
}*/
// Sets speed and a delay value "del" in milliseconds
void setSpeedWithDelay(int speed, int del) {
    setSpeed(speed);
    delay(del);
}

// 0: straight 
void turn(int angle) {
    angle = map(angle, -90, 90, 45, 135);
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
void getGPSData() {
    lat = gps.location.lat();
    lon = gps.location.lng();
    vel = gps.speed.kmph();
    dir = gps.course.deg();
    dataForPi += String(lat, 7) + ";" + String(lon, 7) + ";" + String(vel, 7) + ";" + String(dir, 7);
    IMU_ST_ANGLES_DATA stAngles;
    IMU_ST_SENSOR_DATA stGyroRawData;
    IMU_ST_SENSOR_DATA stAccelRawData;
    IMU_ST_SENSOR_DATA stMagnRawData;
    imuDataGet( &stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);
    yaw = stAngles.fRoll;
    
    
}

// void sendPacket() {
//     String payload = String(lat) + " " + String(lon);
//     payload.toCharArray(gpsCoords, 100);
//     Serial1.write(gpsCoords, 100);
//     memset(gpsCoords, 0, 100);
// }

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
    //String toSend = (setPrecision(7) << lat) + ";" + (setPrecision(7) << lon) + ";" + (setPrecision(7) << boatAngle) + ";" + (setPrecision(7) << velocity);
    sendPiMessage(dataForPi);
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
    
    //if(Serial2.available())
      
    Serial2.readBytes(remoteInstruc, 100);
    
    

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
      y = y_str.toInt();
    if (angle_str != "")
      angle = angle_str.toInt();

    // Displays current values
    /*Serial.println("=====<>=====");
    Serial.print("Manual Control: ");
    Serial.println(IS_MANUAL_CONTROL);
    Serial.println("Function: " + funcCall);
    Serial.print("Y: ");
    Serial.println(y);
    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.println("=====<>=====");*/
    getGPSData();
    

     


    turnAndSetSpeedWithDelay(angle, y, 0);
    
}
