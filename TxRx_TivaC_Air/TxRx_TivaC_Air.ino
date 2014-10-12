#include <SPI.h>
#include <AIR430BoostFCC.h>

#define ADDRESS_LOCAL 0x02
#define ADDRESS_HUB 0x01

struct sPacket
{
  uint8_t node;
  uint8_t msg[59];
};

struct sPacket txPacket;
struct sPacket rxPacket;

// LED lights on Tiva C
int LED[4] = { 
  PN_1, PN_0, PF_4, PF_0 };

// Switches on Tiva C
int SW[2] = { 
  PJ_0, PJ_1 };

void setup()
{
  Serial.begin(115200); 
  Serial.println("INIT");
  // init AIR module
  Radio.begin(0x01, CHANNEL_1, POWER_MAX);

  txPacket.node = ADDRESS_LOCAL;
  memset(txPacket.msg, 0, sizeof(txPacket.msg));

  // start serial communication at 9600 baud


 // Setup LED lights and button switches
  for(int i = 0 ; i < 4 ; i++)
  {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }
  pinMode(SW[0], INPUT_PULLUP);
  pinMode(SW[1], INPUT_PULLUP);
}
void printRxData()
{
  /**
   *  The following illustrates various information that can be obtained when
   *  receiving a message. This includes: the received data and associated 
   *  status information (RSSI, LQI, and CRC_OK bit).
   */
  Serial.print("RX (DATA, RSSI, LQI, CRCBIT): ");
  Serial.print("(");
  Serial.print((char*)rxPacket.msg);
  Serial.print(", ");
  Serial.print(Radio.getRssi());
  Serial.print(", ");
  Serial.print(Radio.getLqi());
  Serial.print(", ");
  Serial.print(Radio.getCrcBit());
  Serial.println(")");
}
void printTxData()
{
  Serial.print("TX (DATA): ");
  Serial.println((char*)txPacket.msg);
}


void loop()
{
  char Temp[] = "Nik Sux";
  
  Serial.println("Scan");
  digitalWrite(LED[0], LOW);
  if (Radio.receiverOn((unsigned char*)&rxPacket, sizeof(rxPacket), 100) > 0)
  {
    Serial.print("FROM: ");
    Serial.print(rxPacket.node);
    Serial.print(" MSG: ");
    Serial.println((char*)rxPacket.msg);
    digitalWrite(LED[0], HIGH);
  }
  digitalWrite(LED[0], LOW);
  
  
  
  
  Serial.println("Transmitt");
  int T = 0;
  
  // Convert String to output format txPacket.msg
  if(sizeof(Temp) < 59)
  {
    for(int i = 0 ; i < sizeof(Temp) ; i++ )
    {
      txPacket.msg[i] = Temp[i];
    }
    for(int i = sizeof(Temp) ; i < 59 ; i++ )
    {
      txPacket.msg[i] = 'N';
    }
  }
  if(sizeof(Temp) >= 59)
  {
    for(int i = 0 ; i < 59 ; i++ )
    {
      txPacket.msg[i] = Temp[i];
    }
  }
  Serial.print("Transmitting: ");
  Serial.print((char*) txPacket.msg);
  Serial.print(" Packet size: ");
  Serial.println(sizeof(txPacket));
  Radio.transmit(ADDRESS_HUB, (unsigned char*)&txPacket, sizeof(txPacket)); 
  
  
}
