#include <Servo.h>
//#include <Arduino_MKRGPS.h>

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo Servo1;
Servo Servo2;
Servo Servo3;
int VRy = A0;
int VRx = A1;
int Btn = 2;

bool IS_MANUAL_CONTROL = false;
int xPos = 0;
int yPos = 0;
int isManual = 0;
int prevIsManual = isManual;
int x = 0;
int y = 0;
int angle = 0.0;
char remoteInstr[100];
char gpsCoords[100];
float lat, lon;

void setup() {
    // Start the serial monitor on port 9600
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(VRx, INPUT);
    pinMode(VRy, INPUT);
    pinMode(Btn, INPUT_PULLUP); 

    // Attach all 3 ESCs
    ESC1.attach(8, 1000, 2000);
    ESC2.attach(9, 1000, 2000);
    ESC3.attach(10, 1000, 2000);
    Servo1.attach(11);
    Servo2.attach(12);
    Servo3.attach(13);
    
    Serial.println("[Starting up...]");
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
    lat = GPS.latitude();
    lon = GPS.longitude();
    
    Serial.println("[Ready!]");
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
void getGPSData() {
    lat = GPS.latitude();
    lon = GPS.longitude();
}

void sendPacket() {
    String payload = String(lat) + " " + String(lon);
    payload.toCharArray(gpsCoords, 100);
    Serial1.write(gpsCoords, 100);
    memset(gpsCoords, 0, 100);
}

void readPacket() {
    Serial.readBytes(remoteInstr, 100);
    // decode and find spaces

    if (remoteInstr[0] == 'F') {
        IS_MANUAL_CONTROL = false;
        return;
    }

    int currSpace, prevSpace = 2;
    int x, angle;
    String x_str, angle_str;
    String funcCall = "";
    int paramNum = 0;
    
    for (int i = 2; i < 100; i++) {
        if (remoteInstr[i] == ' ') {
            currSpace = i;

            switch (paramNum){
                case 0:
                    for (int j = prevSpace; j < currSpace; j++)
                        funcCall += remoteInstr[j];
                    prevSpace = currSpace;
                    paramNum++;
                    break;

                case 1:
                    for (int j = prevSpace; j < currSpace; j++)
                        x_str += remoteInstr[j];
                    x = x_str.toInt();
                    prevSpace = currSpace;
                    paramNum++;
                    break;

                case 2:
                    for (int j = prevSpace; j < currSpace; j++)
                        angle_str += remoteInstr[j];
                    angle = angle_str.toInt();
                    prevSpace = currSpace;
                    paramNum++;
                    break;
            }
        }
    }
    IS_MANUAL_CONTROL = true;
}

void getManualControlData() {
    xPos = analogRead(VRx);
    yPos = analogRead(VRy);
    prevIsManual = isManual;
    isManual = digitalRead(Btn);

    if (xPos > 500 && xPos < 530) xPos = 512;
    if (yPos < 530) yPos = 512;

    x = map(xPos, 0, 1023, -100, 100);
    y = map(yPos, 512, 1023, 0, 100);
    angle = atan2(y, x) * 180 / PI;
    angle = map(angle, 0, 180, 90, -90);

    if (y <= 0) angle = 0.0;

    if (prevIsManual == 0 && isManual == 1) {
        IS_MANUAL_CONTROL = !IS_MANUAL_CONTROL;
    }
}

void loop() {

    // Check if Pi has sent any instructions to Arduino and act on them
    // Idea: from pi to arduino, pass string with the following format:
    // "[function name] [param 1] [param 2] [...param n]"

    // If manual control, listen to radio for commands
    // Else, listen to Pi for commands
    readPacket();
    if (IS_MANUAL_CONTROL) {
        turnAndSetSpeedWithDelay(x, angle, 50);
    } else {
        if (Serial.available() > 0) {   
            String data = Serial.readStringUntil('\n');
            Serial.print("You sent me: ");
            Serial.println(data);
        }
    }
}