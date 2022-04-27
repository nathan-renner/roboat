int VRy = A0;
int VRx = A1;
int Btn = 2;

bool IS_MANUAL_CONTROL = true;
int xPos = 0;
int yPos = 0;
int isManual = 0;
int prevIsManual = isManual;
int x = 0;
int y = 0;
int angle = 0;
char packet[100] = "";

void setup() {
    Serial.begin(9600);
    pinMode(VRx, INPUT);
    pinMode(VRy, INPUT);
    pinMode(Btn, INPUT_PULLUP); 
}

void sendPacket() {
    String payload = "T go " + String(angle) + " " + String(y) + " /";
//    payload.toCharArray(packet, 100);
    Serial.print(payload);
//    Serial.write(packet, 100);
//    memset(packet, 0, 100);
}
//
//void readPacket() {
//    
//}

void getManualControlData() {
    xPos = analogRead(VRx);
    yPos = analogRead(VRy);
    prevIsManual = isManual;
    isManual = digitalRead(Btn);

    if (xPos > 500 && xPos < 530) xPos = 512;
    if (yPos < 530) yPos = 512;

    x = map(xPos, 0, 1023, -100, 100);
    y = map(yPos, 512, 1023, 0, 100);
    angle = atan2(y, x) * 180 / PI;
    angle = map(angle, 0, 180, 90, -90);

    if (y <= 0) angle = 0;

    if (prevIsManual == 0 && isManual == 1) {
        IS_MANUAL_CONTROL = !IS_MANUAL_CONTROL;
    }
}


void loop () {
    getManualControlData();

//     if (IS_MANUAL_CONTROL)
    sendPacket();

    delay(1000);
}