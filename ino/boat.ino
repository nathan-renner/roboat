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
float lat, lon;

int x, angle;
String x_str, angle_str;
String funcCall = "";
int num = 0;

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

// Sets speed and a delay value "del" in milliseconds
void setSpeedWithDelay(int speed, int del) {
    setSpeed(speed);
    delay(del);
}

// 0: straight 
void turn(int angle) {
    angle = map(angle, -90, 90, 0, 180);
    Servo1.write(angle);
    Servo2.write(angle);
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

// Return GPS Coordinats and important IMU data (acceleration)
//void getGPSData() {
//    lat = GPS.latitude();
//    lon = GPS.longitude();
//}

void sendPacket() {
    String payload = String(lat) + " " + String(lon);
    payload.toCharArray(gpsCoords, 100);
    Serial1.write(gpsCoords, 100);
    memset(gpsCoords, 0, 100);
}
//
void readPacket(char remoteInstr[]) {
    //String boatInstr = "";
    //Serial.readBytes(remoteInstr, 100);
    for (int i =0; i<100; i++){
      if(remoteInstr[i] == '/') {
        break;
      }
      boatInstr += remoteInstr[i];
    }
    Serial.println(boatInstr);
    // decode and find spaces

    if (remoteInstr[0] == 'F' || remoteInstr[0] == 'T') {
        Serial.println("Yes");
        
    } else {
      return;
    }
    if (remoteInstr[0] == 'F') {
      IS_MANUAL_CONTROL = false;
      return;
    }

    int currSpace, prevSpace = 2;
    //int x, angle;
    //String x_str, angle_str;
    //String funcCall = "";
    int paramNum = 0;
    
    /*for (int i = 2; i < 100; i++) {
        if (remoteInstr[i] == ' ') {
            currSpace = i;

            switch (paramNum){
                case 0:
                    for (int j = prevSpace; j < currSpace; j++)
                        funcCall += remoteInstr[j];
                    prevSpace = currSpace;
                    Serial.println(funcCall);
                    paramNum++;
                    break;

                case 1:
                    for (int j = prevSpace; j < currSpace; j++)
                        x_str += remoteInstr[j];
                    x = x_str.toInt();
                    prevSpace = currSpace;
                    Serial.println(x);
                    paramNum++;
                    break;

                case 2:
                    for (int j = prevSpace; j < currSpace; j++)
                        angle_str += remoteInstr[j];
                    angle = angle_str.toInt();
                    prevSpace = currSpace;
                    Serial.println(angle);
                    paramNum++;
                    break;
            }
        }
    }*/
    IS_MANUAL_CONTROL = true;
}

void loop() {

    // Check if Pi has sent any instructions to Arduino and act on them
    // Idea: from pi to arduino, pass string with the following format:
    // "[function name] [param 1] [param 2] [...param n]"

    // If manual control, listen to radio for commands
    // Else, listen to Pi for commands
    turnAndSetSpeedWithDelay(45, 50, 1000);
    turnAndSetSpeedWithDelay(0, 70, 1000);
    turnAndSetSpeedWithDelay(0, 0, 2000);
    Serial.readBytes(remoteInstruc, 100);
    //Serial.println(remoteInstruc);
    if(remoteInstruc[0] == 'F' || remoteInstruc[0] == 'T') {
      for(int i = 2; i<100; i++) {
        if(remoteInstruc[i]== '/') {
          break;
        }
        boatInstr += remoteInstruc[i];
      }
    }
    //Serial.println(boatInstr);

    for (int i=0; i<boatInstr.length(); i++) {
      if(boatInstr[i] == ';') {
        num++;
        i++;
      }
      //Serial.println(num);
      if(num == 0) {
        funcCall += boatInstr[i];
      } else if(num == 1){
        x_str += boatInstr[i];
      } else if(num == 2) {
        angle_str += boatInstr[i];
      } else {
        break;
      }
      
    }
    //Serial.println("=========");
    //Serial.println(x_str);
    //Serial.println(angle_str);
    x = x_str.toInt();
    angle = angle_str.toInt();
    Serial.println("=====<>=====");
    Serial.println("Function: " + funcCall);
    Serial.print("X: ");
    Serial.println(x);
    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.println("=====<>=====");
    funcCall = "";
    x_str = "";
    angle_str = "";
    boatInstr = "";
    num =0;
    //readPacket(remoteInstruc);
    //Serial.println( x + " " + angle);
//    if (IS_MANUAL_CONTROL) {
//        turnAndSetSpeedWithDelay(x, angle, 50);
//    } else {
//        if (Serial.available() > 0) {   
//            String data = Serial.readStringUntil('\n');
//            Serial.print("You sent me: ");
//            Serial.println(data);
//        }
//    }
  delay(50);
}
