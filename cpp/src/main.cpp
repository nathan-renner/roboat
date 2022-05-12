#include <iostream>
#include <string.h>
#include <errno.h>
#include <vector>
#include <cmath>
#include <fstream>

//#include <wiringPi.h>
//#include <wiringSerial.h>

//#include <wiringPi.h>

using namespace std;

double location[2];
double heading[2]; //Magnitude, angle

double turnAngle = 0;

double tempWaypoint[2];
vector<double[2]> waypoints;

double calculateAngle(double currentLoc[2], double nextWaypoint[2]) {
    return (atan((currentLoc[0]-nextWaypoint[0])/(currentLoc[1]-nextWaypoint[1]))*180/3.14159265);
}

double calculateDistance(double currentLoc[2], double nextWaypoint[2]) {
    return sqrt(((currentLoc[0]-nextWaypoint[0])*(currentLoc[0]-nextWaypoint[0]))+((currentLoc[1]-nextWaypoint[1])*(currentLoc[1]-nextWaypoint[1])));
}

void turnAndSetSpeedWithDelay(int angle, int speed, int delay = 50) {
    int fd;
    string toSend = angle + ";" + speed;
    if ((fd = serialOpen("/dev/ttyACM0", 9600)) < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return;
    }
    serialPuts(fd, toSend);

    serialClose(fd);
}

void getData(double (&location)[2], double (&heading)[2]) {
    
}

// angle from -90 to 90
// speed from 0 to 100
// delay for current command
//void turnAndSetSpeedWithDelay(int angle, int speed, int delay = 50);

double turn(string type="RESET", string flag="NEGATIVE") { //WIDE, SHARP, VERY SHARP (make this a hashmap?)
    double turningAngle = 0;
    if (type == "VERY SHARP") {
        turningAngle = 30;
    } else if (type == "SHARP") {
        turningAngle = 15;
    } else if (type == "WIDE") {
        turningAngle = 10;
    }
    if (flag == "NEGATIVE") {
        turningAngle = turningAngle*-1;
    }
    //Add code to trigger turning function w/ Arduino
    turnAndSetSpeedWithDelay(turningAngle, 100-turningAngle);
    return turningAngle;
}

int main() {
    //wiringPiSetup();
    //pinMode(0, OUTPUT); // or use whatever pins we want
    //pinMode(1, INPUT);

    // ifstream reader;
    // reader.open("queue.txt");
    // reader.getline(heading, sizeof(heading));

    ifstream reader("queue.txt");
    while (!reader.eof()) {
        reader >> tempWaypoint[0];
        reader >> tempWaypoint[1];
        waypoints.push_back(tempWaypoint);
    }
    turnAndSetSpeedWithDelay(0, 100);
    
    while(true) {
        //string comm;
        //delay(50);
        //Get location and heading from Arduino
        cout << "Location: (" << location[0] << ", " << location[1] << ")" << endl;
        cout << "Heading: " << heading[0] << " at " << heading[1] << " degrees" << endl;
        cout << "Next waypoint: (" << waypoints[0][0] << ", " << waypoints[0][1] << ")" << endl;

        if (calculateDistance(location, waypoints[0]) <= 0.001) {
            waypoints.erase(waypoints.begin());
        }

        if (waypoints.empty()) {
            turnAndSetSpeedWithDelay(0, 0);
        }
        double diff = abs(heading[1]-calculateAngle(location, waypoints[0]));
        string sign;
        if (heading[1] > calculateAngle(location, waypoints[0])) {
            sign = "NEGATIVE";
        } else {
            sign = "POSITIVE";
        }
        if (diff >= 40) {
            turnAngle = turn("VERY SHARP", sign);
        } else if (diff >= 20) {
            turnAngle = turn("SHARP", sign);
        } else if (diff >= 2) {
            turnAngle = turn("WIDE", sign);
        } else {
            turn();
        }
    }
}
