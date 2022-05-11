#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ofstream writer;
    writer.open("data/queue.txt");
    while(true) {
        string temp;
        cout << "Add new waypoint? (y/n)" << endl;
        cin >> temp;
    if (temp == "y") {
            double n[2];
            cout << "Input waypoint x:" << endl;
            cin >> n[0];
            cout << "Input waypoint y:" << endl;
            cin >> n[1];
            writer << n[0] << " ";
            writer << n[1] << " ";
            writer << endl;
        } else {
            writer.close();
            return 0;
        }
    }
}