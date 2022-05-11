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