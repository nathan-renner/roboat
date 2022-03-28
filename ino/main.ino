#include <Servo.h>

Servo ESC;
int sensorValue;

void setup() {
    Serial.begin(9600);
    // pinMode(LED_BUILTIN, OUTPUT);
    ESC.attach(9, 1000, 2000);
    
    Serial.println("0");
    ESC.write(0);
    delay(10000);
}

void loop() {
    // sensorValue = analogRead(A0);
    // int pwmVal = map(sensorValue, 0, 1023, 0, 180);

    Serial.println("10");
    ESC.write(10);
    delay(1000);
    Serial.println("20");
    ESC.write(20);
    delay(1000);
    Serial.println("30");
    ESC.write(30);
    delay(1000);
    Serial.println("40");
    ESC.write(40);
    delay(1000);
    Serial.println("50");
    ESC.write(50);
    delay(1000);
    Serial.println("60");
    ESC.write(60);
    delay(1000);
    Serial.println("70");
    ESC.write(70);
    delay(1000);
    Serial.println("80");
    ESC.write(80);
    delay(1000);
    Serial.println("90");
    ESC.write(90);
    delay(1000);
    Serial.println("100");
    ESC.write(100);
    delay(1000);
    Serial.println("110");
    ESC.write(110);
    delay(1000);
    Serial.println("120");
    ESC.write(120);
    delay(1000);
    Serial.println("130");
    ESC.write(130);
    delay(1000);
    Serial.println("140");
    ESC.write(140);
    delay(1000);
    Serial.println("150");
    ESC.write(150);
    delay(1000);
    Serial.println("0");
    ESC.write(0);
    delay(2000);

    // Serial.print(sensorValue);
    // Serial.print(" ");
    // Serial.print(pwmVal);
    // Serial.println();

    // ESC.write(pwmVal);
}