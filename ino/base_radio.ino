/* Simple Ping-Pong for a LoRa Radio/Modem
 *
 * In setup() below please adjust your country specific frequency ranges,
 * as well as the Bandwidth/SpreadingFactor/CodingRate settings.
 *
 * They way this example works is that the device first listens for 5000ms.
 * If it received a "PING" message, it considers itself a SLAVE. If not
 * it considers itself a MASTER. A SLAVE waits for an incoming "PING" message,
 * which it answers with a "PONG" message. A MASTER simply sends periodically
 * every 1000ms) a "PING" message, and collects "PONG" replies while waiting.
 *    
 *    
 * This example code is in the public domain.
 */
 
#include "LoRaRadio.h"

#define STATE_NONE        0
#define STATE_SCANNING    1
#define STATE_TX_MASTER   2
#define STATE_RX_MASTER   3
#define STATE_TX_SLAVE    4
#define STATE_RX_SLAVE    5

char mystr[100];

int state = STATE_NONE;
String b = "PING";
String message = "Hello World";
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

    state = STATE_SCANNING;

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

    case STATE_SCANNING:
        if ((LoRaRadio.parsePacket() == 4) &&
            (LoRaRadio.read() == 'P') &&
            (LoRaRadio.read() == 'I') &&
            (LoRaRadio.read() == 'N') &&
            (LoRaRadio.read() == 'G'))
        {
            // Got a PING from a master, so we are slave ...
            
            Serial.println("= SLAVE");
            Serial.print("< PING (RSSI: ");
            Serial.print(LoRaRadio.packetRssi());
            Serial.print(", SNR: ");
            Serial.print(LoRaRadio.packetSnr());
            Serial.println(")");
            Serial.println("> PONG");
            
            state = STATE_TX_SLAVE;
            
            LoRaRadio.beginPacket();
            LoRaRadio.write('P');
            LoRaRadio.write('O');
            LoRaRadio.write('N');
            LoRaRadio.write('G');
            LoRaRadio.endPacket();
        }
        else
        {
            if (!LoRaRadio.busy())
            {
                // Didn't hear anything, so we are master ...
                
                Serial.println("= MASTER");
                Serial.println("> PING");
                
                state = STATE_TX_MASTER;

                Package(b);
            }
        }
        break;

    case STATE_TX_MASTER:
        if (!LoRaRadio.busy())
        {
            state = STATE_RX_MASTER;

            LoRaRadio.receive(1000);
        }
        break;

    case STATE_RX_MASTER:
        if (LoRaRadio.parsePacket() > 0) {
            recipient = LoRaRadio.read();
            sender = LoRaRadio.read();
            incomingMsgId = LoRaRadio.read();
            incomingLength = LoRaRadio.read();
            
            Serial.println("==================== ");
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
            Serial.println("Received from: 0x" + String(sender, HEX));
            Serial.println("Sent to: 0x" + String(recipient, HEX));
            Serial.println("Message ID: " + String(incomingMsgId));
            Serial.println("Message length: " + String(incomingLength));
            
            Serial.println("Message: " + incoming);
            incoming = "";
            Serial.println("====================");

        }
        if ((LoRaRadio.parsePacket() == 4) &&
            (LoRaRadio.read() == 'P') &&
            (LoRaRadio.read() == 'O') &&
            (LoRaRadio.read() == 'N') &&
            (LoRaRadio.read() == 'G'))
        {
            // Got a PING from a slave

            Serial.print("< PONG (RSSI: ");
            Serial.print(LoRaRadio.packetRssi());
            Serial.print(", SNR: ");
            Serial.print(LoRaRadio.packetSnr());
            Serial.println(")");
        }

        if (!LoRaRadio.busy())
        {
            // Receive timed out, so send a PING

            Serial.println("> PING");
            
            state = STATE_TX_MASTER;
            Serial1.readBytes(mystr,100);
            Package(mystr);

        }
        break;

    case STATE_TX_SLAVE:
        if (!LoRaRadio.busy())
        {
            state = STATE_RX_SLAVE;

            LoRaRadio.receive();
        }
        break;

    case STATE_RX_SLAVE:
        if ((LoRaRadio.parsePacket() == 4) &&
            (LoRaRadio.read() == 'P') &&
            (LoRaRadio.read() == 'I') &&
            (LoRaRadio.read() == 'N') &&
            (LoRaRadio.read() == 'G'))
        {
            // Got a PING from a master, so send a PONG as reply

            Serial.print("< PING (RSSI: ");
            Serial.print(LoRaRadio.packetRssi());
            Serial.print(", SNR: ");
            Serial.print(LoRaRadio.packetSnr());
            Serial.println(")");
            Serial.println("> PONG");
            
            state = STATE_TX_SLAVE;

            LoRaRadio.beginPacket();
            LoRaRadio.write('P');
            LoRaRadio.write('O');
            LoRaRadio.write('N');
            LoRaRadio.write('G');
            LoRaRadio.endPacket();
        }
        break;
    }
}