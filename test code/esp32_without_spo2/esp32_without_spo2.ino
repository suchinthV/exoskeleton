/*************************************************************

 * this is the code to measure temperature from the temperature probe
    and this esp 32 is connected with blynk
 * first set your mobile hotspot wit PROJECT as the ssid and "project@1" as the password
 * servo motor is also controlled by this esp32
 
    Slider widget (0...100) on V1
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLPwffcLxO"
#define BLYNK_DEVICE_NAME "exoskelton"
#define BLYNK_AUTH_TOKEN "91sGt2ArXpt37pSCTFToXKxQYYz_oC3b"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#define RXD2 16
#define TXD2 17
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
int servo1Pin = 26;
int servo2Pin = 27;
#define ONE_WIRE_BUS 4
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
char ssid[] = "PROJECT";
char pass[] = "project@1";
int pwm;

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 1 angle");
  Serial.println(pinValue);
  myservo1.write(pinValue);
//   for (int pos = 0; pos <= pinValue; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    myservo1.write(pos);    // tell servo to go to position in variable 'pos'
//    delay(15);             // waits 15ms for the servo to reach the position
//  }
}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger2 angle ");
  Serial.println(pinValue);
  myservo2.write(pinValue);
}
BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 3 angle ");
  Serial.println(pinValue);
}BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 4 angle  ");
  Serial.println(pinValue);
}BLYNK_WRITE(V7)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("finger 5 angle  ");
  Serial.println(pinValue);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  sensors.begin();
  Blynk.begin(auth, ssid, pass);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo1.setPeriodHertz(50);    // standard 50 hz servo
  myservo1.attach(servo1Pin, 500, 2400);
  myservo2.setPeriodHertz(50);    // standard 50 hz servo
  myservo2.attach(servo2Pin, 500, 2400);
//    myservo3.setPeriodHertz(50);    // standard 50 hz servo
//  myservo3.attach(servo3Pin, 500, 2400);
//    myservo4.setPeriodHertz(50);    // standard 50 hz servo
//  myservo4.attach(servo4Pin, 500, 2400);
//    myservo5.setPeriodHertz(50);    // standard 50 hz servo
//  myservo5.attach(servo5Pin, 500, 2400);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  sensors.requestTemperatures(); 
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print((char)176);//shows degrees character
  Serial.println("C  |  ");
  Blynk.virtualWrite(V3,sensors.getTempCByIndex(0));

  if (Serial2.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than the sender's because it must store the strings
    StaticJsonDocument<600> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial2);

    if (err == DeserializationError::Ok) 
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.print("bpm = ");
      Serial.println(doc["bpm"].as<int>());
      Serial.print("value = ");
      Serial.println(doc["value"].as<int>());
      Blynk.virtualWrite(V1,doc["bpm"].as<int>());
      Blynk.virtualWrite(V2,doc["value"].as<int>());
    } 
    else 
    {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial2.available() > 0)
        Serial2.read();
    }
  }
//  delay(100);/
}
