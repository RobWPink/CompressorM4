#include <ModbusMaster_oneH2.h>
#include <Arduino.h>
#include <RPC.h>
#define RHE28_ADDR 3
#define RED_ADDR 1
ModbusMaster mbRTU;
void preTransmission() {;}
void postTransmission() {;}
bool tog = false;
String msg = "";
float rhe28[22] = {0};
float red[4] = {0};
void setup() {
  Serial.begin(9600);
  Serial4.begin(19200, SERIAL_8N1); //default RHE28 is 57600
  mbRTU.begin(1, Serial4);
  mbRTU.preTransmission(preTransmission);
  mbRTU.postTransmission(postTransmission);
}

void loop() {
  if(!tog){
    msg = "{RHE28:";
    uint8_t result = mbRTU.readInputRegisters(RHE28_ADDR,0x5000,44);
    if (result == mbRTU.ku8MBSuccess) {
      rhe28[0] = word2Float(mbRTU.getResponseBuffer(0),mbRTU.getResponseBuffer(1),true);
      rhe28[1] = word2Float(mbRTU.getResponseBuffer(2),mbRTU.getResponseBuffer(3),true);
      rhe28[2] = word2Float(mbRTU.getResponseBuffer(4),mbRTU.getResponseBuffer(5),true);
      rhe28[3] = word2Float(mbRTU.getResponseBuffer(6),mbRTU.getResponseBuffer(7),true);
      for(int i = 4; i < 22; i++){
        rhe28[i] = word2Float(mbRTU.getResponseBuffer(i*2),mbRTU.getResponseBuffer(i*2+1),true);
      }
      mbRTU.clearResponseBuffer();
      for(int i : rhe28){
        msg = msg + String(rhe28[i]) + ",";
      }
      msg[msg.length()-1] = '}';
      RPC.println(msg);
    }
    else{RPC.print(msg);RPC.print(result,HEX);RPC.println("}");}
    delay(500);
    tog = 1;
  }
  else{
    msg = "{RED:";
    uint8_t result = mbRTU.readHoldingRegisters(RED_ADDR,0,8);
    if (result == mbRTU.ku8MBSuccess) {
      red[0] = word2Float(mbRTU.getResponseBuffer(0),mbRTU.getResponseBuffer(1),false);
      red[1] = word2Float(mbRTU.getResponseBuffer(2),mbRTU.getResponseBuffer(3),false);
      red[2] = word2Float(mbRTU.getResponseBuffer(4),mbRTU.getResponseBuffer(5),false);
      red[3] = word2Float(mbRTU.getResponseBuffer(6),mbRTU.getResponseBuffer(7),false);
      mbRTU.clearResponseBuffer();
      for(int i : red){
        msg = msg + String(red[i]) + ",";
      }
      msg[msg.length()-1] = '}';
      RPC.println(msg);
    }
    else{RPC.print(msg);RPC.print(result,HEX);RPC.println("}");}
    delay(500);
    tog = 0;
  }
}

float word2Float(uint16_t a, uint16_t b, bool littleBig){ //set littleBig to true to do big endian, otherwise its little endian
  union flreg_t {
    float asFloat;
    uint16_t asInt[2];
  };
  flreg_t flreg = { 0 };
  flreg.asInt[0] = littleBig?b:a;  //Swap to convert to Little Endian
  flreg.asInt[1] = littleBig?a:b;
  return flreg.asFloat;
}


// flow = word2Float(mbRTU.getResponseBuffer(0),mbRTU.getResponseBuffer(1));
// temp = word2Float(mbRTU.getResponseBuffer(2),mbRTU.getResponseBuffer(3));
// pressure = word2Float(mbRTU.getResponseBuffer(4),mbRTU.getResponseBuffer(5));
// total = word2Float(mbRTU.getResponseBuffer(6),mbRTU.getResponseBuffer(7));


// RHE28 data
// uint32_t errorStatus = word2Float(mbRTU.getResponseBuffer(0),mbRTU.getResponseBuffer(1),true);
// uint32_t SoftError = word2Float(mbRTU.getResponseBuffer(2),mbRTU.getResponseBuffer(3),true);
// uint32_t Warnings = word2Float(mbRTU.getResponseBuffer(4),mbRTU.getResponseBuffer(5),true);
// uint32_t InfoStatus = word2Float(mbRTU.getResponseBuffer(6),mbRTU.getResponseBuffer(7),true);
// float DenComp = word2Float(mbRTU.getResponseBuffer(8),mbRTU.getResponseBuffer(9),true);
// float MassFlowRate = word2Float(mbRTU.getResponseBuffer(10),mbRTU.getResponseBuffer(11),true);
// float VolumetricFlowRate = word2Float(mbRTU.getResponseBuffer(12),mbRTU.getResponseBuffer(13),true);
// float TotalMassFwd = word2Float(mbRTU.getResponseBuffer(14),mbRTU.getResponseBuffer(15),true);
// float TotalVolFwd  = word2Float(mbRTU.getResponseBuffer(16),mbRTU.getResponseBuffer(17),true);
// float TotInvenMassNet = word2Float(mbRTU.getResponseBuffer(18),mbRTU.getResponseBuffer(19),true);
// float TotInvenVolNet = word2Float(mbRTU.getResponseBuffer(20),mbRTU.getResponseBuffer(21),true);
// float TotalMassRev = word2Float(mbRTU.getResponseBuffer(22),mbRTU.getResponseBuffer(23),true);
// float TotalVolRev = word2Float(mbRTU.getResponseBuffer(24),mbRTU.getResponseBuffer(25),true);
// float AdcTubeMeanTemp = word2Float(mbRTU.getResponseBuffer(26),mbRTU.getResponseBuffer(27),true);
// float AdcTorBarMeanTemp = word2Float(mbRTU.getResponseBuffer(28),mbRTU.getResponseBuffer(29),true);
// float OnBrdTemp = word2Float(mbRTU.getResponseBuffer(30),mbRTU.getResponseBuffer(31),true);
// float PrsMean = word2Float(mbRTU.getResponseBuffer(32),mbRTU.getResponseBuffer(33),true);
// float AssuranceFactor = word2Float(mbRTU.getResponseBuffer(34),mbRTU.getResponseBuffer(35),true);
// float StdDensity = word2Float(mbRTU.getResponseBuffer(36),mbRTU.getResponseBuffer(37),true);
// float VolPercentMainSubstance = word2Float(mbRTU.getResponseBuffer(38),mbRTU.getResponseBuffer(39),true);
// float MassFlowRateDisplay = word2Float(mbRTU.getResponseBuffer(40),mbRTU.getResponseBuffer(41),true);
// float VolFlowRateDisplay = word2Float(mbRTU.getResponseBuffer(42),mbRTU.getResponseBuffer(43),true);