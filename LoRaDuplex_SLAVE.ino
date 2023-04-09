/*
  LoRa Duplex communication

  Sends a message every half second, and polls continually
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.

  Uses readString() from Stream class to read payload. The Stream class'
  timeout may affect other functuons, like the radio's callback. For an

  created 28 JAN 2023     
  by Madhan Kumar Chiruguri
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <ESP32Servo.h>
#include <EEPROM.h>
#include "EEPROM.h"
Servo servo1;
Servo servo2;

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define VOLTAGE_SENSOR 12 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0;
// Float for Reference Voltage
float ref_voltage = 3.3; 
// Integer for ADC value
int adc_value = 0;


#define LED1 22
#define LED2 15

const int csPin = 5;          // LoRa radio chip select
const int resetPin = 14;       // LoRa radio reset
const int irqPin = 2;         // change for your board; must be a hardware interrupt pin

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 1000;          // interval between sends




  int joy1_state;
  int joy2_state;
  int joy3_state;
  int joy4_state;
  
  int btn1_state;
  int btn2_state;
  int btn3_state;
  int btn4_state;
  
  int servo1_angle;
  int servo2_angle;
  float temp;
  float humi;

//dc motors
int in1 = 25;
int in2 = 26;

int servo1Dir_address = 22;
int servo2Dir_address = 23;

int motor1Dir_address = 24;
int motor2Dir_address = 25;
int motor3Dir_address = 26;
int motor4Dir_address = 27;
byte servo1Dir_value, servo2Dir_value;
byte motor1Dir_value,motor2Dir_value,motor3Dir_value,motor4Dir_value;

  
void setup() {
  Serial.begin(115200);                   // initialize serial
  EEPROM.begin(255);
  while (!Serial);
  dht.begin();

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

 
  Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");



// Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);// Standard 50hz servo
  servo2.setPeriodHertz(50);// Standard 50hz servo
  servo1.attach(32, 500, 2400);   // attaches the servo on pin 18 to the servo object
  servo2.attach(33, 500, 2400);   // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"


  servo1Dir_value = EEPROM.read(servo1Dir_address);//EEPROM.read(ADDRESS 0-255);
  servo2Dir_value = EEPROM.read(servo2Dir_address);//EEPROM.read(ADDRESS 0-255);

  motor1Dir_value = EEPROM.read(motor1Dir_address);//EEPROM.read(ADDRESS 0-255);
  motor2Dir_value = EEPROM.read(motor2Dir_address);//EEPROM.read(ADDRESS 0-255);
  motor3Dir_value = EEPROM.read(motor3Dir_address);//EEPROM.read(ADDRESS 0-255);
  motor4Dir_value = EEPROM.read(motor4Dir_address);//EEPROM.read(ADDRESS 0-255);



}





void loop() {
  //sending data to master node
//  sendDataToMaster();
 

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}




//void sendDataToMaster(){
//String SendJSONData = "";
//StaticJsonDocument<1024> doc;
//
//
//  humi = dht.readHumidity();
//  // Read temperature as Celsius (the default)
//  temp = dht.readTemperature();
//  // Read temperature as Fahrenheit (isFahrenheit = true)
////  float fah = dht.readTemperature(true);
//  
//  adc_value = analogRead(VOLTAGE_SENSOR);
//  // Determine voltage at ADC input
//  adc_voltage  = (adc_value * ref_voltage) / 4095;
//  // Calculate voltage at divider input
//  in_voltage = adc_voltage / (R2 / (R1 + R2)) ;
////  Serial.print("Input Voltage = ");
////  Serial.println(in_voltage, 2);
//  
//  doc["sw1_state"] = sw1_state;
//  doc["sw2_state"] = sw2_state;
//  doc["joyx_state"] = joyx_state;
//  doc["joyy_state"] = joyy_state;
//  doc["servo1_angle"] = servo1_angle;
//  doc["servo2_angle"] = servo2_angle;
//  doc["temp"] = temp;
//  doc["humi"] =  humi;
//  doc["distance"] = (int)random(5) + 30.50;
//  doc["volt"] = in_voltage;
//  // Add an array.
//  //
//  JsonArray data = doc.createNestedArray("cordinates");
//  data.add(48.756080);
//  data.add(2.302038);
//  
//serializeJson(doc, SendJSONData);
////serializeJsonPretty(doc, SendJSONData);
//
//  unsigned long currentMillis = millis();
//  if (currentMillis - lastSendTime > interval) { //sending every 1 second
//    lastSendTime = currentMillis; 
//  Serial.println("Sending " + SendJSONData);
//  LoRa.beginPacket();                   // start packet
//  LoRa.write(destination);              // add destination address
//  LoRa.write(localAddress);             // add sender address
//  LoRa.write(msgCount);                 // add message ID
//  LoRa.write(SendJSONData.length());        // add payload length
//  LoRa.print(SendJSONData);                 // add payload
//  LoRa.endPacket();                     // finish packet and send it
//  msgCount++;                           // increment message ID
//  }   
//
//
//}






void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }



StaticJsonDocument<1024> doc2;
  DeserializationError error = deserializeJson(doc2, incoming);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
//  const char* sensor = doc2["sensor"];
//  long time = doc2["time"];
  int joy1 = doc2["joysticks"][0];
  int joy2 = doc2["joysticks"][1];
  int joy3 = doc2["joysticks"][2];
  int joy4 = doc2["joysticks"][3];

  int pot1 = doc2["potentiometers"][0];
  int pot2 = doc2["potentiometers"][1];

  int btn1 = doc2["button"][0];
  int btn2 = doc2["button"][1];
  int btn3 = doc2["button"][2];
  int btn4 = doc2["button"][3];

int motor1 = doc2["motor"][0];
int motor2 = doc2["motor"][1];
int motor3 = doc2["motor"][2];
int motor4 = doc2["motor"][3];
int servos1 = doc2["servo"][0];
int servos2 = doc2["servo"][1];


    EEPROM.write(motor1Dir_address, motor1);
    EEPROM.write(motor2Dir_address, motor2);
    EEPROM.write(motor3Dir_address, motor3);
    EEPROM.write(motor4Dir_address, motor4);
    EEPROM.write(servo1Dir_address, servos1);
    EEPROM.write(servo2Dir_address, servos2);
    EEPROM.commit();

Serial.println("******************************************************************************");      
Serial.println("Servo1 angle:" + String(servo1Dir_value) + ", Servo2 angle:" + String(servo2Dir_value));
Serial.println("Motor1:" + String(motor1Dir_value)+", Motor2:" + String(motor2Dir_value)+", Motor3:" + String(motor3Dir_value)+", Motor4:" + String(motor4Dir_value));
Serial.println("******************************************************************************");      
Serial.println();

if(btn1 == 1){
  btn1_state = 1;
  digitalWrite(LED1, HIGH);
//  Serial.println("led1 on");
}else{
  btn1_state = 0;
  digitalWrite(LED1, LOW);
//  Serial.println("led1 off");
}

if(btn2 == 1){
  btn2_state = 1; 
  digitalWrite(LED2, HIGH);
//  Serial.println("led2 on");
}else{
  btn2_state = 0;
  digitalWrite(LED2, LOW);
//  Serial.println("led2 off");
}

//sw1_state = (sw1_state == 1)?0:1
//sw2_state = (sw2_state == 1)?0:1
joy1_state = joy1;
joy2_state = joy2;
joy3_state = joy3;
joy4_state = joy4;

servo1.write(pot1);
servo2.write(pot2);
servo1_angle = pot1;
servo2_angle = pot2;





if(joy1_state > 130){
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
}
else if(joy1_state < 120){
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
}
else{
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);  
}



  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
//  Serial.println("Switch1: " + sw1);
//  Serial.println("Switch2: " + sw2);
//  Serial.println("Joystick_x: " + joyx);
//  Serial.println("Joystick_y: " + joyy);
  Serial.println();



  
}
