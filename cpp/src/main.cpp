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