#include <analogWrite.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "NIZrHnsGxvrdqIiu2CqhDDk8pGfFhCG0";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OnePlus 5T";
char pass[] = "c813da74egys";

/* ESP32 to Driver module*/
/* Motor A*/
const int ENA = 32;
const int IN1 = 33;
const int IN2 = 25;

/* Motor B*/
const int IN3 = 27;
const int IN4 = 14;
const int ENB = 12;

// Function for Motor Control
void motorControl(int x, int y){
int motorSpeed1 = 0;
int motorSpeed2 = 0;
  
// Forward
  if (y>522 && y<1023){
    digitalWrite(IN1,HIGH); // MotorA
    digitalWrite(IN2,LOW);
    
    digitalWrite(IN3,HIGH); //MotorB
    digitalWrite(IN4,LOW);
  
    motorSpeed1 = map(y, 522, 1023, 0, 255);
    motorSpeed2 = map(y, 522, 1023, 0, 255);
    analogWrite(ENA, motorSpeed1);
    analogWrite(ENB, motorSpeed2);
  }

// Backward
  else if (y<502 && y>0){
    digitalWrite(IN1,LOW); // MotorA
    digitalWrite(IN2,HIGH); 
  
    digitalWrite(IN3,LOW); // MotorB
    digitalWrite(IN4,HIGH); 
  
    motorSpeed1 = map(y, 502, 0, 0, 255);
    motorSpeed2 = map(y, 502, 0, 0, 255);  
    analogWrite(ENA, motorSpeed1);
    analogWrite(ENB, motorSpeed2);  
  }

// stop if forward and backward condition is not satisfied 
  else{
    motorSpeed1 = 0;
    motorSpeed2 = 0;
    analogWrite(ENA, motorSpeed1);
    analogWrite(ENB, motorSpeed2);
  }
  

// Left
  if (x<502 && x>0){
    x = map(x, 502, 0, 0, 255);
    motorSpeed1 =  motorSpeed1 - x;  // Differential Speed - Left wheel rotates slower
    motorSpeed2 =  motorSpeed2 + x;  // Differential Speed - Right wheel rotates faster  

    if (motorSpeed1 < 0){
      motorSpeed1 = 0;}
    if (motorSpeed2 > 255){
      motorSpeed2 = 255;}
    analogWrite(ENA, motorSpeed1);
    analogWrite(ENB, motorSpeed2);  
  }

// Right
  else if(x>522 && x<1023){
    x = map(x, 522, 1023, 0, 255);
    motorSpeed1 = motorSpeed1 + x;  // Differential Speed - Left wheel rotates faster
    motorSpeed2 = motorSpeed2 + x;  // Differential Speed - Right wheel rotates slower   
    if (motorSpeed1 > 255){
      motorSpeed1 = 255;}
    if (motorSpeed2 < 0){
      motorSpeed2 = 0;}
      analogWrite(ENA, motorSpeed1);
      analogWrite(ENB, motorSpeed2);
     }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  
  Blynk.begin(auth, ssid, pass);

  Serial.println("Connecting");             /*Checking if wifi is connected, if not "." is printed every 500 ms*/
  while(WiFi.status() != WL_CONNECTED) { 
  delay(500);
  Serial.print(".");
  }

  // Once WiFi is connected, the SSID is printed
  Serial.print("");
  Serial.print("ESP 32 connected to WiFi");
  Serial.print(ssid);
  Serial.println();
  
  /* Driver Module*/
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);
}

void loop()
{
  Blynk.run();
}

// BLYNK_WRITE is a function called every time device gets an update of Virtual Pin value from the server (or app):
// BLYNK_WRITE can’t be used inside of any loop or function. It’s a standalone function.
BLYNK_WRITE(V1)
{   
  int value = param.asInt(); // Get value as integer
  
  // The param can contain multiple values, in such case:
  int x = param[0].asInt();
  int y = param[1].asInt();
  motorControl(x, y);
}
