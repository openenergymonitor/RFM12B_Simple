//Simple RFM12B wireless demo - Receiver - no ack
//Glyn Hudson openenergymonitor.org GNU GPL V3 12/4/12
//Credit to JCW from Jeelabs.org for RFM12 

#include <JeeLib.h>

#define myNodeID 30          //node ID of Rx (range 0-30) 
#define network     210      //network group (can be in the range 1-250).
#define RF_freq RF12_433MHZ     //Freq of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ. Match freq to module

typedef struct { int power1, power2, power3, battery; } PayloadTX;      // create structure - a neat way of packaging data for RF comms
PayloadTX emontx;  

const int emonTx_NodeID=10;            //emonTx node ID

void setup() {
  
  rf12_initialize(myNodeID,RF_freq,network);   //Initialize RFM12 with settings defined above  
  Serial.begin(9600); 
  Serial.println("RF12B demo Receiver - Simple demo"); 
  
 Serial.print("Node: "); 
 Serial.print(myNodeID); 
 Serial.print(" Freq: "); 
 if (RF_freq == RF12_433MHZ) Serial.print("433Mhz");
 if (RF_freq == RF12_868MHZ) Serial.print("868Mhz");
 if (RF_freq == RF12_915MHZ) Serial.print("915Mhz");  
 Serial.print(" Network: "); 
 Serial.println(network);
}

void loop() {
  
 if (rf12_recvDone()){    
  if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
    
    int node_id = (rf12_hdr & 0x1F);		  //extract nodeID from payload
        
    if (node_id == emonTx_NodeID)  {             //check data is coming from node with the corrct ID
        emontx=*(PayloadTX*) rf12_data;            // Extract the data from the payload 
       Serial.print("power1: "); Serial.println(emontx.power1); 
       Serial.print("power2: "); Serial.println(emontx.power2); 
       Serial.print("power3: "); Serial.println(emontx.power3); 
       Serial.print("battery: "); Serial.println(emontx.battery); 
       Serial.println("  "); 
  }
 }
}
}
