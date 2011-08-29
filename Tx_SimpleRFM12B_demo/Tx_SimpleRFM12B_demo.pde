//Simple RFM12B wireless demo - transimtter - no ack
//Glyn Hudson openenergymonitor.org GNU GPL V3 7/7/11
//Credit to JCW from Jeelabs.org for RFM12 

#include <RF12.h>
#include <Ports.h> //from jeelabs.org

#define myNodeID 10          //node ID of tx (range 0-30)
#define network     212      //network group (can be in the range 1-250).
#define freq RF12_433MHZ     //Freq of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ. Match freq to module

#define RADIO_SYNC_MODE 2 //sync mode to 2 if fuses are Arduino default. Mode 3, full powerdown) only used with 258 CK startup fuse

#define COLLECT 0x20 // collect mode, i.e. pass incoming without sending acks


typedef struct {		//data Structure to be sent, called payload
  	  int data1;		// current transformer 1]
	  float data2;		// emontx voltage
} Payload;

Payload demo;

void setup() {
  rf12_initialize(myNodeID,freq,network);   //Initialize RFM12 with settings defined above  
Serial.begin(9600);
Serial.println("RFM12B Transmitter - Simple demo");

 Serial.print("Node: "); 
  Serial.print(myNodeID); 
  Serial.print(" Freq: "); 
   if (freq == RF12_433MHZ) Serial.print("433Mhz");
   if (freq == RF12_868MHZ) Serial.print("868Mhz");
   if (freq == RF12_915MHZ) Serial.print("915Mhz"); 
  Serial.print(" Network: "); 
  Serial.println(network);
}

void loop() {
  demo.data1=demo.data1+1;
  demo.data2=demo.data2+0.2; 
  
  while (!rf12_canSend())
    rf12_recvDone();
    rf12_sendStart(rf12_hdr, &demo, sizeof demo, RADIO_SYNC_MODE); 

Serial.print(demo.data1); Serial.print(" ");
Serial.println(demo.data2);
delay(1000);
}
