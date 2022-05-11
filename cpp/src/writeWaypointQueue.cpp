#include <fstream>
#include <iostream>

using namespace std;

main() {
    ofstream writer;
    while(true) {
        string temp;
        cout << "Add new waypoint? (y/n)" << endl;
        cin >> temp;
    if (temp == "y") {
            writer.open("queue.txt");
            double n[2];
            cout << "Input waypoint x:" << endl;
            cin >> n[0];
            cout << "Input waypoint y:" << endl;
            cin >> n[1];
            writer << n[0];
            writer << n[1];
            writer.close();
        }
    }
}