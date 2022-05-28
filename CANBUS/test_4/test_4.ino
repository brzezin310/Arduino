// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}

void loop()
{
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    for(int i = 0; i < 4096; i++){
    //int x  = analogRead(A0)/4;
    //unsigned char stmp[8] = {x,x,x,x,x};
//unsigned char stmp[8] = {255,255,255,255,255,255,255,255};
  //unsigned char stmp[8] = {164,164,164,164};
    //unsigned char stmp[8] = {192,192,192,192,192,192,192,192};
    unsigned char stmp[8] = {255,255,255,255,255,255,255,255};
   //unsigned char stmp[8] = {100,100,100,100,100,100,100,100};
//unsigned char stmp[8] = {64,64,64,64,64,64,64,64};
   //unsigned char stmp[8] = {32,32,32,32,32,32,32,32};
    //unsigned char stmp[8] = {0,x,0,0,0,0,0,0};
    CAN.sendMsgBuf(i,0, 8, stmp);

    //delay(100); 
    //Serial.println(x);
    
    Serial.println(i);
}// send data per 100ms
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
