#include <Servo.h>

//Yellow- 2
//Red- 3
//Brown- 4

Servo ESC;

void setup() {
  ESC.attach(8, 1000, 2000);
  ESC.write(180);
  delay(2000);
  ESC.write(0);
  delay(2000);
}

void loop() {
  ESC.write(90);
  delay(3000);
}
