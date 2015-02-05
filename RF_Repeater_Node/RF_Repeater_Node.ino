// RFM12B / RFM69CW RF Repeater
// Usefull to increase range on networks

/*Recommended node ID allocation
------------------------------------------------------------------------------------------------------------
-ID-	-Node Type- 
0	- Special allocation in JeeLib RFM12 driver - reserved for OOK use
1-4     - Control nodes 
5-10	- Energy monitoring nodes
11-14	--Un-assigned --
15-16	- Base Station & logging nodes
17-30	- Environmental sensing nodes (temperature humidity etc.)
31	- Special allocation in JeeLib RFM12 driver - Node31 can communicate with nodes on any network group
------------------------------------------------------------------------------------------------------------
*/

#define RF69_COMPAT 0                     // Set to 1 if using RFM69CW or 0 is using RFM12B
#include <JeeLib.h>                       //https://github.com/jcw/jeelib     

const int Rx_NetworkGroup = 210;       
const int Tx_NetworkGroup = 200;
const int nodeID = 11;                  //node ID of repeter node (not visible) 
#define RF_freq RF12_433MHZ             // Frequency can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ. 

const int LEDpin=6;
byte buf[66];


void setup() {

  pinMode(LEDpin, OUTPUT); 
  digitalWrite(LEDpin,HIGH); 
  
  Serial.begin(9600);
  Serial.println("RFM12B Repeater node"); 
  Serial.println("OpenEnergyMonitor.org");
  
  Serial.print("Node: "); Serial.print(nodeID); 
    Serial.print(" Freq: "); 
    if (RF_freq == RF12_433MHZ) Serial.print("433Mhz");
    if (RF_freq == RF12_868MHZ) Serial.print("868Mhz");
    if (RF_freq == RF12_915MHZ) Serial.print("915Mhz"); 
    Serial.print("Rx Network: "); Serial.println(Rx_NetworkGroup);
    Serial.print("Tx Network: "); Serial.println(Tx_NetworkGroup);
   delay(500);  
  
  rf12_initialize(nodeIDF,RF_freq,Rx_NetworkGroup);     //Node ID, Freq, receive network group

  digitalWrite(LEDpin,LOW); 
}

void loop()
{ 
  wdt_reset();
  if (rf12_recvDone())
  {
    if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0)  // and no rf errors
    {
      int node_id_Rx = (rf12_hdr & 0x1F);                  //node ID of received packert
      
      if (node_id > 1 && node_id<31 && node_id!=16)
      {
        digitalWrite(LEDpin,HIGH); 
        Serial.println("Data recieved");
        byte len = rf12_len;
        memcpy(buf, (void*) rf12_data, rf12_len);
        
        rf12_initialize(node_id_Rx,RF_freq,Tx_NetworkGroup); //put RF into transmit mode on Tx network group 
        
        int i = 0; while (!rf12_canSend() && i<10) {rf12_recvDone(); i++;}
        rf12_sendStart(0, buf, len);
        rf12_sendWait(0); 
        
        rf12_initialize(nodeID,RF_freq,Rx_NetworkGroup);    //put RF back into Receive mode on Rx network group
        digitalWrite(LEDpin,LOW); 
      }
    }
  }
   
}

