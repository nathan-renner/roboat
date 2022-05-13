#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>

using namespace std;

class commModule {
private:
    struct termios tty;
    int port;

public:
    commModule(string line="/dev/ttyACM0") {
        port = open(line.c_str(), O_RDWR);
        if (port < 0) {
            printf("Error %i from open: %s\n", errno, strerror(errno));
        }
        if(tcgetattr(port, &tty) != 0) {
            printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        }
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag |= CREAD | CLOCAL;
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO; // Disable echo
        tty.c_lflag &= ~ECHOE; // Disable erasure
        tty.c_lflag &= ~ECHONL; // Disable new-line echo
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
        tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        tty.c_cc[VMIN] = 0;
        cfsetispeed(&tty, B9600);
        cfsetospeed(&tty, B9600);
        if (tcsetattr(port, TCSANOW, &tty) != 0) {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
    }
    void turnAndSetSpeedWithDelay(int angle, int speed, int delay = 50) {
        string toSend = angle + ";" + speed;
        write(port, toSend.c_str(), sizeof(toSend.c_str()));
    }

    int getData(double (&location)[2], double (&heading)[2]) {
        string request = "R"; //For "Requesting"
        string buffer_converted;
        char buffer[39];
        int errcode;
        write(port, request.c_str(), sizeof(request.c_str()));
        errcode = read(port, &buffer, sizeof(buffer));
        buffer_converted = buffer;
        stringstream finder(buffer_converted.substr(0, buffer_converted.find_first_of(";")));
        finder >> location[0];
        buffer_converted.erase(0, buffer_converted.find_first_of(";")+1);
        finder = stringstream(buffer_converted.substr(0, buffer_converted.find_first_of(";")));
        finder >> location[1];
        buffer_converted.erase(0, buffer_converted.find_first_of(";")+1);
        finder = stringstream(buffer_converted.substr(0, buffer_converted.find_first_of(";")));
        finder >> heading[0];
        buffer_converted.erase(0, buffer_converted.find_first_of(";")+1);
        finder = stringstream(buffer_converted.substr(0, buffer_converted.find_first_of(";")));
        finder >> heading[1];
        buffer_converted.erase(0, buffer_converted.find_first_of(";")+1);
        return errcode;
    }
};