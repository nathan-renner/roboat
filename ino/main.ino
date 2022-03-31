#include <Servo.h>

Servo ESC1;
Servo ESC2;
Servo ESC3;
char dataReceived[50];

void setup() {
    // Start the serial monitor on port 9600
    Serial.begin(9600);

    // Attach all 3 ESCs
    ESC1.attach(9, 1000, 2000);
    // ESC2.attach(10, 1000, 2000);
    // ESC3.attach(11, 1000, 2000);
    
    // Calibrate the ESCs by holding 
    Serial.println("[Starting up...]");
    ESC1.write(0);
    // ESC2.write(0);
    // ESC3.write(0);
    delay(10000);
    Serial.println("[Ready!]");
}

// Takes in a value "speed" from 0 to 100 (maps from 0 to 180)
// and a delay value "del" in milliseconds
void setMotorSpeed(int speed, int del) {
    Serial.print("Speed: ");
    Serial.println(speed);
    int calcSpeed = map(speed, 0, 100, 0, 180);
    ESC1.write(calcSpeed);
    // ESC2.write(calcSpeed);
    // ESC3.write(calcSpeed);
    delay(del);
}

void loop() {

    // NOT FINISHED! Check if Pi has sent any instructions to Arduino and act on them
    // Idea: from pi to arduino, pass string with the following format:
    // "[function name] [param 1] [param 2] [...param n]"

    // if (Serial.available() > 0) {   
    //     String dataReceived = Serial.readStringUntil('\n');
    //     char *funcHead = strtok(dataReceived, " ");
    // }

    setMotorSpeed(50, 1000);
    setMotorSpeed(0, 1000);

    // The below code will eventually be modified for manual override control 
    // int sensorValue = analogRead(A0);
    // int pwmVal = map(sensorValue, 0, 1023, 0, 180);
    // ESC.write(pwmVal);
}