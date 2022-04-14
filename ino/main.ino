#include <Servo.h>

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo Servo1;
Servo Servo2;
Servo Servo3;

void setup() {
    // Start the serial monitor on port 9600
    Serial.begin(9600);

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
    Serial.println("[Ready!]");
}

// Takes in a value "speed" from 0 to 100 (maps from 0 to 180)
void setSpeed(int speed) {
    speed = map(speed, 0, 100, 0, 180);
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
void getSensorData() {

}

void loop() {

    // NOT FINISHED! Check if Pi has sent any instructions to Arduino and act on them
    // Idea: from pi to arduino, pass string with the following format:
    // "[function name] [param 1] [param 2] [...param n]"

    if (Serial.available() > 0) {   
        String data = Serial.readStringUntil('\n');
        Serial.print("You sent me: ");
        Serial.println(data);
    }


    // Simulate a turn (timing not accurate)
    // turnAndSetSpeedWithDelay(0, 60, 2000);
    // turnAndSetSpeedWithDelay(30, 50, 2000);
    // turnAndSetSpeedWithDelay(45, 30, 2000);
    // turnAndSetSpeedWithDelay(90, 20, 2000);
    // turnAndSetSpeedWithDelay(45, 30, 2000);
    // turnAndSetSpeedWithDelay(30, 50, 2000);
}