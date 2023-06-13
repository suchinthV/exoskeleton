

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
int beatAvg;
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

void setup() {
pinMode(4,INPUT); // put your setup code here, to run once:
pinMode(red,OUTPUT);
pinMode(blue,OUTPUT);
pinMode(green,OUTPUT);
 digitalWrite(red,LOW);
 digitalWrite(green,LOW);
 digitalWrite(blue,LOW);

attachInterrupt(4, isr, RISING);
Serial.begin(9600);
 if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  
}

void loop() {
if(mode==1){
   long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.println(beatAvg);

  if (irValue < 50000){
    Serial.println(" No finger?");
    
  }
}
else if(mode==2){
  
}

}
