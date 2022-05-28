#include <Servo.h>
//dwa serwa

Servo myservo;
Servo myservo1;
Servo myservo2;

int xValue = 0 ;
int yValue = 0 ; 
int bValue = 0 ;

void setup() 
{
  myservo.attach(2); 
  myservo1.attach(3); 
  myservo2.attach(5);
  Serial.begin(9600) ;
  pinMode(8,INPUT); 
  digitalWrite(8,HIGH);
}

void loop() 
{
  xValue = analogRead(A0);  
  yValue = analogRead(A1);  
  bValue = digitalRead(8);  
  myservo.write(xValue/6);
  myservo1.write(yValue/6);
  myservo2.write(yValue/6);
}
