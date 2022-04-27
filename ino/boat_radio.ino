#include "LoRaRadio.h"

#define STATE_NONE        0
#define STATE_SCANNING    1
#define STATE_TX_MASTER   2
#define STATE_RX_MASTER   3
#define STATE_TX_SLAVE    4
#define STATE_RX_SLAVE    5

char mystr[100];
char payload[100];

int state = STATE_NONE;


byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to

int recipient;           // recipient address
byte sender;             // sender address
byte incomingMsgId;      // incoming msg ID
byte incomingLength;    // incoming msg length
String incoming = "";

void setup( void )

{
    
    Serial.begin(9600);
    
    while (!Serial) { }

    LoRaRadio.begin(915000000);

    LoRaRadio.setFrequency(915000000);
    LoRaRadio.setTxPower(14);
    LoRaRadio.setBandwidth(LoRaRadio.BW_125);
    LoRaRadio.setSpreadingFactor(LoRaRadio.SF_7);
    LoRaRadio.setCodingRate(LoRaRadio.CR_4_5);
    LoRaRadio.setLnaBoost(true);

    state = STATE_TX_MASTER;

    LoRaRadio.receive(5000);
}

void Package (String a) {

  LoRaRadio.beginPacket();
  LoRaRadio.write(destination);
  LoRaRadio.write(localAddress);
  LoRaRadio.write(msgCount);          
  LoRaRadio.write(a.length());
  LoRaRadio.print(a);
  LoRaRadio.endPacket();
  msgCount++;
}

void loop( void )
{
    switch (state) {
    case STATE_NONE:
        break;


    case STATE_TX_MASTER:
        if (!LoRaRadio.busy())
        {
            state = STATE_RX_MASTER;

            LoRaRadio.receive();
        }
        break;

    case STATE_RX_MASTER:
        if (LoRaRadio.parsePacket() > 0) {
            recipient = LoRaRadio.read();
            sender = LoRaRadio.read();
            incomingMsgId = LoRaRadio.read();
            incomingLength = LoRaRadio.read();
            
            //Serial.println("==================== ");
            while (LoRaRadio.available()) {
            incoming += (char)LoRaRadio.read();
            }
            if (incomingLength != incoming.length()) {   // check length for error
              Serial.println("error: message length does not match length");
              return;                             // skip rest of function
            }

            // if the recipient isn't this device or broadcast,
            if (recipient != localAddress && recipient != 0xFF) {
              Serial.println("This message is not for me.");
              return;                             // skip rest of function
            }   
            //Serial.println("Received from: 0x" + String(sender, HEX));
            //Serial.println("Sent to: 0x" + String(recipient, HEX));
            //Serial.println("Message ID: " + String(incomingMsgId));
            //Serial.println("Message length: " + String(incomingLength));
            
            //Serial.print(/*"Message: " + */incoming);
            incoming.toCharArray(payload, 100);
            Serial.write(payload, 100);
            incoming = "";
            //Serial.println("====================");
            delay(50);

        }

        if (!LoRaRadio.busy())
        {
            // Receive timed out, so send a PING

            //Serial.println("> PING");
            
            state = STATE_TX_MASTER;
            //Serial.readBytes(mystr,100);
            //Serial.println("====");
            //Serial.println(mystr);
            //Serial.println("====");
            //Package(mystr);

        }
        break;


    }
}
