#include <mcp_can.h>
#include <SPI.h>
#define lo8(x) ((int)(x)&0xff)
#define hi8(x) ((int)(x)>>8)


const int SPI_CS_PIN = 9;
//#################liczniki petlowe#################
int c_1;
int c_2;
int c_3;
int c_4;
int c_5;
int c_6;
//#################wejscia cyfrowe#################
const int D0 = 0;
const int D1 = 1;
const int D3 = 3;
const int D4 = 4;
const int D5 = 5;
const int D6 = 6;
const int D7 = 7;
const int D8 = 8;
const int D10 = 10;

byte air_bag = 0;
byte ABS = 0;

int buttonstate = 0;

MCP_CAN CAN(SPI_CS_PIN);

//#################obroty silnika ################# 
 
unsigned int rpm = 0;  // odczyt z wyjścia A0

byte rpmLOW = 0;       // konwersja na bajty starsze

byte rpmHIGH = 0;      // konwersja na bajty młodsze

//##############predkosc pojazdu##################

unsigned int vehicle_speed = 0;

byte speedLOW = 0;

byte speedHIGH = 0;

unsigned int lightning_power = 0;

//##############temperatura silnika############

unsigned int  coolant = 0;

//################kierunkowskazy_akumulator_czujnik otwarcia drzwi########################

unsigned int     turning_light = 0;
unsigned int     door_sensor = 0;

byte x = 0;


void setup()
{

    pinMode(D0, INPUT_PULLUP);
    pinMode(D1, INPUT_PULLUP);
    pinMode(D3, INPUT_PULLUP);
    pinMode(D4, INPUT_PULLUP);
    pinMode(D5, INPUT_PULLUP);
    pinMode(D6, INPUT_PULLUP);
    pinMode(D7, INPUT_PULLUP);
    pinMode(D8, INPUT_PULLUP);
    pinMode(D10, INPUT_PULLUP);
  Serial.begin(500000);

START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS))                  
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

   //#################ODCZYT DANYCH#####################
//#################obroty silnika#################
    DDRD = B11111111;

    rpm = analogRead(A0)*30;
    
//#################wartość natezenia oswietlenia#################

    lightning_power = analogRead(A2)*0.10;
    
//#################predkosc pojazdu#################

    vehicle_speed = analogRead(A1)*0.15;

//#################kontrola temperatury silnika#################

    if(coolant = analogRead(A3)>250)
    {
      coolant = 250;
    }else if((coolant = analogRead(A3)<120))
    {
      coolant = 120;
    }else
    {
      (coolant = analogRead(A3));
    }

//#################konwersja bitowa##################################

    rpmLOW = lo8(rpm);
    rpmHIGH = hi8(rpm);

    speedLOW = lo8(vehicle_speed);
    speedHIGH = hi8(vehicle_speed);

Serial.println(turning_light);


    if (Serial.available()) 
    {
      sterowanie(); 
    }else{
      program0();
    }

//#########################################################################3#
    
//#################kierunkowskazy + wskaznik otwarcia drzwi + podswietlenie#################

    unsigned char frame_0[] = {turning_light,door_sensor,lightning_power,0};
    CAN.sendMsgBuf(0x470, 0, 8, frame_0);

    //#########################CAN_01 ==> moduł świateł#########################
   

      unsigned char stmp15[] = {0,0,0,0,0,0,0,0};
      CAN.sendMsgBuf(0x9A0, 0, 8, stmp15);
      
      unsigned char frame_2[] = {0,0,0,0,254,254,0,0};
      CAN.sendMsgBuf(0x1A0, 0, 8, frame_2);

      unsigned char frame_5[] = {0,0,0,0};
      CAN.sendMsgBuf(0x2A0, 0, 4, frame_5);

      unsigned char frame_3[] = {127,0,vehicle_speed,0,0,0,(vehicle_speed/4),0};
      CAN.sendMsgBuf(0x5A0, 0, 8, frame_3);

      unsigned char frame_4[] = {0,0,0,0,0,0,0};
      CAN.sendMsgBuf(0x4A0, 0, 8, frame_4);
 
//#############################DO IDENTYFIKACJI####################################  


 //DEC 1504
      unsigned char frame_6[] = {33,0,255,0,0,0,128,0};
      CAN.sendMsgBuf(0x5E0, 0, 8, frame_6);
   
 //DEC 208   
      unsigned char frame_7[] = {104,96,4,12,0,0};
      CAN.sendMsgBuf(0xD0, 0, 6, frame_7);

 //DEC 976
      unsigned char frame_8[] = {4,130,8,16,200,91};
      CAN.sendMsgBuf(0x3D0, 0, 8, frame_8);
     
 //DEC 1488 
      unsigned char frame_9[] = {192,3,80,143,51,81,0,0};
      CAN.sendMsgBuf(0x5D0, 0, 8, frame_9);

 //DEC 1490
      unsigned char frame_10[] = {1,65,65,55,49,75,56,56};
      CAN.sendMsgBuf(0x5D2, 0, 8, frame_10);


 //DEC 1416
      unsigned char frame_11[] = {232,0,124,0,0,64,0,0};
      CAN.sendMsgBuf(0x588, 0, 8, frame_11);

 //DEC 800
      unsigned char frame_12[] = {1,0,0,1,0,3,0,129};
      CAN.sendMsgBuf(0x320, 0, 8, frame_12);

 //DEC 1394
      unsigned char frame_13[] = {17};
      CAN.sendMsgBuf(0x572, 0, 1, frame_13);

 //DEC 194
      unsigned char frame_14[8] = {0,128,0,128,0,0,0,0};
      CAN.sendMsgBuf(0xC2, 0, 8, frame_14);
 //DEC 896
      unsigned char frame_15[8] = {0,101,0,0,128,0,0,0};
      CAN.sendMsgBuf(0x380, 0, 8, frame_15);
 //DEC 1160
      unsigned char frame_16[8] = {69,0,0,124,166,0,255,96};
      CAN.sendMsgBuf(0x488, 0, 8, frame_16);
 //DEC 1500
      unsigned char frame_17[8] = {117,0,96,15,30,0,0,64};
      CAN.sendMsgBuf(0x5DC, 0, 8, frame_17);
 //DEC 906
      unsigned char frame_22[] = {236,0,236,0};
      CAN.sendMsgBuf(0x38A, 0, 4, frame_22);
//DEC 1312   
      unsigned char frame_23[] = {2,2,0,147,76,75,242,2};
      CAN.sendMsgBuf(0x520, 0, 8, frame_23);
//DEC 1408
      unsigned char frame_26[] = {152,0,0,42,24,125,88,153};
      CAN.sendMsgBuf(0x580, 0, 8, frame_26);
//DEC 1392
      unsigned char frame_27[] = {16,32,168,0,0};
      CAN.sendMsgBuf(0x570, 0, 5, frame_27);
            

//#############################SILNIK#####################################################
//###############obroty 4 bit ustala wartość obrotow silnika##############################
//DEC 640
      unsigned char frame_19[] = {0,0,rpmLOW,rpmHIGH,0,0,0,0};
      CAN.sendMsgBuf(0x280, 0, 8, frame_19);
//@@@@@@@@@@@@@@@@@@@@@@@@czujnik wody_bit drugi@@@@@@@@@@@@@@@@@@@@@@@@@
//DEC 648
      unsigned char frame_20[] = {50,coolant,34,64,64,64,64,64};
      CAN.sendMsgBuf(0x288, 0, 8, frame_20);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//DEC 488
      unsigned char frame_21[] = {0,0,0,124,166,0,0,0};
      CAN.sendMsgBuf(0x1E8, 0, 8, frame_21);
//DEC 420
      unsigned char frame_18[] = {147,255,255,0,200,0,255,128};
      CAN.sendMsgBuf(0x1A4, 0, 8, frame_18);

//DEC 1478     
      unsigned char frame_24[] = {0,36,34,0,0,16,32,3};
      CAN.sendMsgBuf(0x5C6, 0, 8, frame_24);
//DEC 480     
      unsigned char frame_25[] = {24,0,0,0,26,0,2,114};
      CAN.sendMsgBuf(0x1E0, 0, 8, frame_25);


      
      //**************AIRBAG******************
       unsigned char stmp13[8] = {air_bag,air_bag,air_bag,air_bag};
       CAN.sendMsgBuf(0x050, 0, 8, stmp13);
      //**************AIRBAG******************
      

      delay(20);
}
//########################################################################
//##############################podprogramy###############################
void sterowanie()
{
  
 if(digitalRead(D0) == LOW){
  kierunkowskaz_lewy();
 }else if(digitalRead(D1) == LOW){
  kierunkowskaz_prawy();
 }else if(digitalRead(D3) == LOW){
  awaryjne();
 }else if(digitalRead(D4) == LOW){
  kierunkowskaz_lewy_przyczepy();
 }else if(digitalRead(D5) == LOW){
  kierunkowskaz_prawy_przyczepy();
 }else if(digitalRead(D6) == LOW){
  awaryjne_przyczepy();
 }else if(digitalRead(D7) == LOW){
  door_sensor = 1;
 }else if(digitalRead(D8) == LOW){
  turning_light = 200;
 }else if(digitalRead(D10) == LOW){
  air_bag = 255;
 }else{
  program0();
  ABS = 0;
  air_bag = 0;
 }


}
void program0()
{
        turning_light = 0;
        c_1 = 0;
        c_2 = 0;
        c_3 = 0;
        c_4 = 0;
        c_5 = 0;
        c_6 = 0;
        door_sensor = 0;
}
//############### kierunkowskaz lewy###############
void kierunkowskaz_lewy()
{     
      if(c_1 == 7)
      {
        turning_light = 1;
        c_1 = c_1 + 1;
      }else if(c_1 == 14)
      {
        turning_light = 0;
        c_1 = 0;
      }else{
        c_1 = c_1 + 1;
      }
}
//###############kierunkowskaz prawy###############
void kierunkowskaz_prawy()
{
      if(c_2 == 7)
      {
        turning_light = 2;
        c_2 = c_2 + 1;
      }else if(c_2 == 14)
      {
        turning_light = 0;
        c_2 = 0;
      }else{
        c_2 = c_2 + 1;
      }
}
//###############swiatła awaryjne###############
void awaryjne()
{ 
      if(c_3 == 7)
      {
        turning_light = 3;
        c_3 = c_3 + 1;
      }else if(c_3 == 14)
      {
        turning_light = 0;
        c_3 = 0;
      }else{
        c_3 = c_3 + 1;
      }
}
//###############kierunkowskaz lewy przyczepy###############
void kierunkowskaz_lewy_przyczepy()
{ 
      if(c_4 == 7)
      {
        turning_light = 5;
        c_4 = c_4 + 1;
      }else if(c_4 == 14)
      {
        turning_light = 0;
        c_4 = 0;
      }else{
        c_4 = c_4 + 1;
      }
}
//###############kierunkowskaz prawy przyczepy###############
void kierunkowskaz_prawy_przyczepy()
{ 
      if(c_5 == 7)
      {
        turning_light = 6;
        c_5 = c_5 + 1;
      }else if(c_5 == 14)
      {
        turning_light = 0;
        c_5 = 0;
      }else{
        c_5 = c_5 + 1;
      }
}
//############### światła awaryjne przyczepy###############
void awaryjne_przyczepy()
{ 
      if(c_6 == 7)
      {
        turning_light = 7;
        c_6 = c_6 + 1;
      }else if(c_6 == 14)
      {
        turning_light = 0;
        c_6 = 0;
      }else{
        c_6 = c_6 + 1;
      }
}
//#################Airbag######################################



