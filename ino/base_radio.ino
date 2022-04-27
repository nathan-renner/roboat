#include "LoRaRadio.h"

#define STATE_NONE        0
#define STATE_SCANNING    1
#define STATE_TX_MASTER   2
#define STATE_RX_MASTER   3
#define STATE_TX_SLAVE    4
#define STATE_RX_SLAVE    5

char mystr[100];
char payload[100];
String instr;
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

    state = STATE_TX_SLAVE;

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
    
  
  Serial.readBytes(mystr,100);
  //Serial.println("====");
  for (int i = 0; i <100; i++) {
    if (mystr[i] == '/') {
      for (int j=0; j<=i; j++){
        instr += mystr[j];  
      }
      break;
      
    }
  }
  Serial.println(instr);
  
  //Serial.println("====");
  Package(instr);
  instr = "";
  delay(50);

    
}
