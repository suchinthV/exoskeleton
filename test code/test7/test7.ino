 

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLPwffcLxO"
#define BLYNK_DEVICE_NAME "exoskelton"
#define BLYNK_AUTH_TOKEN "91sGt2ArXpt37pSCTFToXKxQYYz_oC3b"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
int servo1Pin = 27;
int servo2Pin = 26;
int servo3Pin = 25;
int servo4Pin = 19;
int servo5Pin = 23;
int Mode=0;

#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "wifi";
char pass[] = "password";
int pwm;

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 1 angle");
  Serial.println(pinValue);
  myservo1.write(pinValue);

}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger2 angle ");
  Serial.println(pinValue);
  myservo2.write(pinValue);
}
BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 3 angle ");
  Serial.println(pinValue);
  myservo3.write(pinValue);
} 
BLYNK_WRITE(V7)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 4 angle  ");
  Serial.println(pinValue);
  myservo4.write(pinValue);
} 
BLYNK_WRITE(V8)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 5 angle  ");
  Serial.println(pinValue);
  myservo5.write(pinValue);
}
BLYNK_WRITE(V9)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("mode 1  ");
  Serial.println(pinValue);
  if(pinValue==1)
  {
    Mode=1;
  }
  else if(pinValue==0)
  {
    Mode=0;
  }
}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("mode 2 ");
  Serial.println(pinValue);
  if(pinValue==1)
  {
    Mode=2;
  }
   else if(pinValue==0)
  {
    Mode=3;
  }
}
#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

unsigned long time1;

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg ,count1=0;
volatile int count=0,mode=0;
#define red 14
#define green 13
#define blue 12
#define Switch 4
void IRAM_ATTR isr() {
    count++;

if (count<3 ){
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
  digitalWrite(blue,LOW);
  mode=1;
}
else if(count<6){
digitalWrite(green,HIGH);
 digitalWrite(red,LOW);
  digitalWrite(blue,LOW);
  mode=2;
}
else if(count<9)
{
  digitalWrite(blue,HIGH);
   digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  mode=3;
}
else if(count>10)
{
  count=0;
    digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  digitalWrite(blue,LOW);
  mode=0;
}
   
}




void setup()
{
   pinMode(4,INPUT); // put your setup code here, to run once:
 attachInterrupt(4, isr, RISING);
pinMode(red,OUTPUT);
pinMode(blue,OUTPUT);
pinMode(green,OUTPUT);
 digitalWrite(red,HIGH);
 digitalWrite(green,HIGH);
 digitalWrite(blue,LOW);
  // Debug console
  Serial.begin(115200);
  sensors.begin();
  Blynk.begin(auth, ssid, pass);
  
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo1.setPeriodHertz(50);    // standard 50 hz servo
  myservo1.attach(servo1Pin, 500, 2400);
  myservo2.setPeriodHertz(50);    // standard 50 hz servo
  myservo2.attach(servo2Pin, 500, 2400);
  myservo3.setPeriodHertz(50);    // standard 50 hz servo
  myservo3.attach(servo3Pin, 500, 2400);
  myservo4.setPeriodHertz(50);    // standard 50 hz servo
  myservo4.attach(servo4Pin, 500, 2400);
  myservo5.setPeriodHertz(50);    // standard 50 hz servo
  myservo5.attach(servo5Pin, 500, 2400);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
 


 if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
 
 digitalWrite(red,LOW);
 digitalWrite(green,LOW);
 digitalWrite(blue,LOW);

}

void loop()
{
//  if(mode==2){
////    Serial.println("mode2");
//   long irValue = particleSensor.getIR();
//
//  if (checkForBeat(irValue) == true)
//  {
//    //We sensed a beat!
//    long delta = millis() - lastBeat;
//    lastBeat = millis();
//
//    beatsPerMinute = 60 / (delta / 1000.0);
//
//    if (beatsPerMinute < 255 && beatsPerMinute > 20)
//    {
//      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
//      rateSpot %= RATE_SIZE; //Wrap variable
//
//      //Take average of readings
//      beatAvg = 0;
//      for (byte x = 0 ; x < RATE_SIZE ; x++)
//        beatAvg += rates[x];
//      beatAvg /= RATE_SIZE;
//    }
//  }

//  Serial.print("IR=");
//  Serial.print(irValue);
//  Serial.print(", BPM=");
//  Serial.print(beatsPerMinute);
//  Serial.print(", Avg BPM=");
//  Serial.println(beatAvg);
//count1++;
// if(count1>100)
// {
//  count1=0;
Blynk.virtualWrite(V1, beatAvg);
// }
//  if (irValue < 50000){
//    Serial.println(" No finger?");
//    
//  }
//}
//else if(mode==1){
  Blynk.run();
//  Serial.println("mode1");
  sensors.requestTemperatures();
  Serial.println(sensors.getTempCByIndex(0));
  Blynk.virtualWrite(V3, sensors.getTempCByIndex(0));

  
  if(Mode==1){
    for(int i=0 ;i<180 ;i++){
      myservo1.write(i);
      delay(10);
    }
    for(int i=0 ;i<180 ;i++){
      myservo2.write(i);
      delay(10);
    }for(int i=0 ;i<180 ;i++){
      myservo3.write(i);
      delay(10);
    }
    for(int i=0 ;i<180 ;i++){
      myservo4.write(i);
      delay(10);
    }
    for(int i=0 ;i<180 ;i++){
      myservo5.write(i);
      delay(10);
    }

    
     for(int i=180 ;i>0 ;i--){
      myservo1.write(i);
      delay(10);
    }
     for(int i=180 ;i>0 ;i--){
      myservo2.write(i);
      delay(10);
    }
     for(int i=180 ;i>0 ;i--){
      myservo3.write(i);
      delay(10);
    }
     for(int i=180 ;i>0 ;i--){
      myservo4.write(i);
      delay(10);
    }
     for(int i=180 ;i>0 ;i--){
      myservo5.write(i);
      delay(10);
    }
  }
  else if(Mode == 2){
     for(int i=0 ;i<150 ;i++){
      myservo1.write(i);
       myservo2.write(i);
        myservo3.write(i);
         myservo4.write(i);
          myservo5.write(i);
      delay(30);
    }

     for(int i=150 ;i>0 ;i--){
      myservo1.write(i);
       myservo2.write(i);
        myservo3.write(i);
         myservo4.write(i);
          myservo5.write(i);
      delay(30);
    }
  }
}
