#include <iostream>
#include <string.h>
#include <errno.h>

#include <wiringSerial.h> // (installed on pi)

using namespace std;

int main() {
    int x = 2;
    int y = x + 4;
    cout << "Hello world" << y << endl;
}