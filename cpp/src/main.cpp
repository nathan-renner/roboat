#include <iostream>
#include <string.h>
#include <errno.h>
#include <vector>
#include <cmath>
#include <fstream>

// #include <wiringSerial.h> (installed on pi)
//Let's add this to our rep

using namespace std;

float location[2];
float heading[2]; //Magnitude, angle

float marginOfError = 1;

// angle from -90 to 90
// speed from 0 to 100
// delay for current command
void turnAndSetSpeedWithDelay(int angle, int speed, int delay = 50);

void turn(string type="WIDE") { //WIDE, SHARP, VERY SHARP (make this a hashmap?)
    float turningAngle = 0;
    if (type == "VERY SHARP") {
        turningAngle = 30;
    } else if (type == "SHARP") {
        turningAngle = 15;
    } else {
        turningAngle = 10;
    }
    //Add code to trigger turning function w/ Arduino
}

int main() {
    ifstream reader;
    reader.open("queue.txt");
    reader.getline(heading, sizeof(heading));

    while(true) {
        //Get location and heading from Arduino
        cout << "Location: (" << location[0] << ", " << location[1] << ")" << endl;
        cout << "Heading: " << heading[0] << " at " << heading[1] << " degrees" << endl;

    }
}

// Below is the code to communicate to Arduino via USB Serial

// Use "g++ roboat.cpp -lwiringPi" to run. 
// Don't forget "-lwiringPi" or it won't work.
// #include <stdio.h>
// #include <string.h>
// #include <errno.h>
// #include <wiringSerial.h>

// int main () {
//     int fd;

//     if ((fd = serialOpen("/dev/ttyACM0, 9600")) < 0) {
//         fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
//         return 1;
//     }
//     for (;;) {
//         putchar(serialGetchar(fd));
//         fflush(stdout);
//     }
// }
