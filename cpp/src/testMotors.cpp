#include "commModule.h"

using namespace std;

commModule arduino = commModule();

int main() {
    char flag = 'N';
    while (flag != 'K') {
        flag = arduino.start();
    }

    arduino.turnAndSetSpeedWithDelay(45, 30);
    sleep(1);
    arduino.turnAndSetSpeedWithDelay(0, 0);
}