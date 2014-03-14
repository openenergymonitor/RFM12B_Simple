//Simple RFM12B wireless demo - Receiver - no ack
//Glyn Hudson openenergymonitor.org GNU GPL V3 12/4/12
//Credit to JCW from Jeelabs.org for RFM12 

#include <JeeLib.h>              //use on emonTx V2 / JeeNode
//#include <RFu_JeeLib.h>       //use on emonTx V3 with RFU328

#define myNodeID 30          //node ID of Rx (range 0-30) 
#define network     210      //network group (can be in the range 1-250).
#define RF_freq RF12_433MHZ     //Freq of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ. Match freq to module

typedef struct { int power1, power2, power3, power4, Vrms, temp;  } PayloadTX;      // create structure - a neat way of packaging data for RF comms
PayloadTX emontx;  

//const int LED_pin=6;      //use on emonTx V3
const int LED_pin= 9;       //emonTx V2 

void setup() {
  
  pinMode(OUTPUT, LED_pin);
  digitalWrite(LED_pin, HIGH);
  
  rf12_initialize(myNodeID,RF_freq,network);   //Initialize RFM12 with settings defined above  
  Serial.begin(9600); 
  Serial.println("RF12B demo Receiver - Simple demo - emonTx V3"); 
  
 Serial.print("Node: "); 
 Serial.print(myNodeID); 
 Serial.print(" Freq: "); 
 if (RF_freq == RF12_433MHZ) Serial.print("433Mhz");
 if (RF_freq == RF12_868MHZ) Serial.print("868Mhz");
 if (RF_freq == RF12_915MHZ) Serial.print("915Mhz");  
 Serial.print(" Network: "); 
 Serial.println(network);
 digitalWrite(LED_pin, LOW);
}

void loop() {
  
 if (rf12_recvDone()){    
  if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
    
    digitalWrite(LED_pin, HIGH);
    int node_id = (rf12_hdr & 0x1F);		  //extract nodeID from payload
        
        emontx=*(PayloadTX*) rf12_data;            // Extract the data from the payload 
        Serial.print(node_id); Serial.print("  "); 
       Serial.print(emontx.power1); Serial.print("  "); 
       Serial.print(emontx.power2); Serial.print("  "); 
       Serial.print(emontx.power3); Serial.print("  "); 
       Serial.print(emontx.power4); Serial.print("  "); 
       Serial.print(emontx.Vrms); Serial.print("  "); 
       Serial.println(emontx.temp); 
       
       digitalWrite(LED_pin, LOW);
       
  }
 }
}

