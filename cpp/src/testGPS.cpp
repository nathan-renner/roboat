#include <iostream>
#include "commModule.h"

using namespace std;

commModule arduino = commModule();

int main() {
    double loc[2];
    double head[2];
    char flag = 'N';
    while (flag != 'K') {
        flag = arduino.start();
    }
    arduino.getData(loc, head);
    cout << loc[0] << "," << loc[1] << endl;
    cout << head[0] << "," << head[1] << endl;
}