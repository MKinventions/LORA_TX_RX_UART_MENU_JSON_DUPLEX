#include <ArduinoJson.h>
#include <EEPROM.h>
#include "EEPROM.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SPI.h>              // include libraries
#include <LoRa.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define RXp2 16
#define TXp2 17

//lora esp32
#define NSS 5
#define RST 14
#define DIO0 2
String loraOutgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to
unsigned long lastSendTime = 0;
int interval = 10;          // interval between sends



const int nextPageButton = 1;
int pagesCount = 0;
int totalPages = 6;

int potentiometer1 = 26;
int potentiometer2 = 27;
int joystick1 = 35;
int joystick2 = 39;
int joystick3 = 34;
int joystick4 = 36;
int joybutton1 = 13;
int joybutton2 = 12;

int encoder1_DT = 33;
int encoder1_CLK = 32;
int encoder1_btn = 25;

int encoder2_DT = 15;
int encoder2_CLK = 4;
int encoder2_btn = 3;

int potentiometer1Value, potentiometer2Value, joystick1Value, joystick2Value, joystick3Value, joystick4Value;
int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;

int encoder1_counter = 0;
int currentStateCLK1;
int lastStateCLK1;

int encoder2_counter = 0;
int currentStateCLK2;
int lastStateCLK2;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;
int lastButton1 = 0;
int lastButton2 = 0;
int lastButton3 = 0;
int lastButton4 = 0;


int pot1_address = 1;
int pot2_address = 2;
int pot3_address = 3;
int pot4_address = 4;
int joy1_address = 5;
int joy2_address = 6;
int joy3_address = 7;
int joy4_address = 8;

int pot1Dir_address = 9;
int pot2Dir_address = 10;
int pot3Dir_address = 11;
int pot4Dir_address = 12;
int joy1Dir_address = 13;
int joy2Dir_address = 14;
int joy3Dir_address = 15;
int joy4Dir_address = 16;

int btn1Dir_address = 18;
int btn2Dir_address = 19;
int btn3Dir_address = 20;
int btn4Dir_address = 21; 

int servo1Dir_address = 22;
int servo2Dir_address = 23;

int motor1Dir_address = 24;
int motor2Dir_address = 25;
int motor3Dir_address = 26;
int motor4Dir_address = 27;

byte pot1_value, pot2_value, pot3_value, pot4_value, joy1_value, joy2_value, joy3_value, joy4_value;
byte pot1Dir_value, pot2Dir_value, pot3Dir_value, pot4Dir_value, joy1Dir_value, joy2Dir_value, joy3Dir_value, joy4Dir_value;

byte btn1Dir_value,btn2Dir_value,btn3Dir_value,btn4Dir_value;
byte servo1Dir_value, servo2Dir_value;
byte motor1Dir_value,motor2Dir_value,motor3Dir_value,motor4Dir_value;

String servo1Dir_Type, servo2Dir_Type;
String motor1Dir_Type, motor2Dir_Type, motor3Dir_Type, motor4Dir_Type;

void(* resetFunc) (void) = 0; //declare reset function @ address 0


void setup() {
    Wire.begin(); // join i2c bus (address optional for master)

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(40, 30);
  display.println("Restarted");
  display.display();

pinMode(nextPageButton, INPUT_PULLUP);
pinMode(potentiometer1, INPUT);
pinMode(potentiometer2, INPUT);
pinMode(joystick1, INPUT);
pinMode(joystick2, INPUT);
pinMode(joystick3, INPUT);
pinMode(joystick4, INPUT);
pinMode(joybutton1, INPUT_PULLUP);
pinMode(joybutton2, INPUT_PULLUP);
pinMode(encoder1_CLK,INPUT_PULLUP);
pinMode(encoder1_DT,INPUT_PULLUP);
pinMode(encoder1_btn, INPUT_PULLUP);
pinMode(encoder2_CLK,INPUT_PULLUP);
pinMode(encoder2_DT,INPUT_PULLUP);
pinMode(encoder2_btn, INPUT_PULLUP);


  EEPROM.begin(255);
  pot1_value = EEPROM.read(pot1_address);//EEPROM.read(ADDRESS 0-255);
  pot2_value = EEPROM.read(pot2_address);//EEPROM.read(ADDRESS 0-255);
  pot3_value = EEPROM.read(pot3_address);//EEPROM.read(ADDRESS 0-255);
  pot4_value = EEPROM.read(pot4_address);//EEPROM.read(ADDRESS 0-255);
  joy1_value = EEPROM.read(joy1_address);//EEPROM.read(ADDRESS 0-255);
  joy2_value = EEPROM.read(joy2_address);//EEPROM.read(ADDRESS 0-255);
  joy3_value = EEPROM.read(joy3_address);//EEPROM.read(ADDRESS 0-255);
  joy4_value = EEPROM.read(joy4_address);//EEPROM.read(ADDRESS 0-255);

  pot1Dir_value = EEPROM.read(pot1Dir_address);//EEPROM.read(ADDRESS 0-255);
  pot2Dir_value = EEPROM.read(pot2Dir_address);//EEPROM.read(ADDRESS 0-255);
  pot3Dir_value = EEPROM.read(pot3Dir_address);//EEPROM.read(ADDRESS 0-255);
  pot4Dir_value = EEPROM.read(pot4Dir_address);//EEPROM.read(ADDRESS 0-255);
  joy1Dir_value = EEPROM.read(joy1Dir_address);//EEPROM.read(ADDRESS 0-255);
  joy2Dir_value = EEPROM.read(joy2Dir_address);//EEPROM.read(ADDRESS 0-255);
  joy3Dir_value = EEPROM.read(joy3Dir_address);//EEPROM.read(ADDRESS 0-255);
  joy4Dir_value = EEPROM.read(joy4Dir_address);//EEPROM.read(ADDRESS 0-255); 

  btn1Dir_value = EEPROM.read(btn1Dir_address);//EEPROM.read(ADDRESS 0-255);
  btn2Dir_value = EEPROM.read(btn2Dir_address);//EEPROM.read(ADDRESS 0-255);
  btn3Dir_value = EEPROM.read(btn3Dir_address);//EEPROM.read(ADDRESS 0-255);
  btn4Dir_value = EEPROM.read(btn4Dir_address);//EEPROM.read(ADDRESS 0-255);

  servo1Dir_value = EEPROM.read(servo1Dir_address);//EEPROM.read(ADDRESS 0-255);
  servo2Dir_value = EEPROM.read(servo2Dir_address);//EEPROM.read(ADDRESS 0-255);

  motor1Dir_value = EEPROM.read(motor1Dir_address);//EEPROM.read(ADDRESS 0-255);
  motor2Dir_value = EEPROM.read(motor2Dir_address);//EEPROM.read(ADDRESS 0-255);
  motor3Dir_value = EEPROM.read(motor3Dir_address);//EEPROM.read(ADDRESS 0-255);
  motor4Dir_value = EEPROM.read(motor4Dir_address);//EEPROM.read(ADDRESS 0-255);
  
//  Serial.println("Read =  btn1:" + String(btn1Dir_value)+", btn2:" + String(btn2Dir_value)+", btn3:" + String(btn3Dir_value)+", btn4:" + String(btn4Dir_value));
//  Serial.println("Read =  joy1:" + String(joy1_value)+"["+String(joy1Dir_value)+"], joy2:" + String(joy2_value)+"["+String(joy2Dir_value)+"], joy3:" + String(joy3_value)+"["+String(joy3Dir_value)+"], joy4:" + String(joy4_value)+"["+String(joy4Dir_value)+"]");
//  Serial.println("Read =  pot1:" + String(pot1_value)+"["+String(pot1Dir_value)+"], pot2:" + String(pot2_value)+"["+String(pot2Dir_value)+"], pot3:" + String(pot3_value)+"["+String(pot3Dir_value)+"], pot4:" + String(pot4_value)+"["+String(pot4Dir_value)+"]");


  // Read the initial state of CLK
  lastStateCLK1 = digitalRead(encoder1_CLK);
  lastStateCLK2 = digitalRead(encoder2_CLK);


    Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
LoRa.setPins(NSS, RST, DIO0);

  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");



String SendJSONData = "";
StaticJsonDocument<1024> doc;

   JsonArray motor = doc.createNestedArray("motor");
    motor.add(motor1Dir_value);
    motor.add(motor2Dir_value);
    motor.add(motor3Dir_value);
    motor.add(motor4Dir_value);
            
    JsonArray servo = doc.createNestedArray("servo");
    servo.add(servo1Dir_value);
    servo.add(servo2Dir_value);
   

serializeJson(doc, SendJSONData);

//serializeJsonPretty(doc, SendJSONData);
  Serial.println("Sending " + SendJSONData);
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(SendJSONData.length());        // add payload length
  LoRa.print(SendJSONData);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
  
}

void loop() {
 UART_RECEIVER_DATA();
 
   //sending data to slave node lora
  sendDataToSlave();
  
//  UART_TRANSMITTER_DATA();

/*********potentiometer1************/
     if(pot1_value == 180 & pot1Dir_value == 0){ potentiometer1Value = map(analogRead(potentiometer1), 0, 4095, 0, 180); }
else if(pot1_value == 180 & pot1Dir_value == 1){ potentiometer1Value = map(analogRead(potentiometer1), 0, 4095, 180, 0); }
else if(pot1_value == 255 & pot1Dir_value == 0){ potentiometer1Value = map(analogRead(potentiometer1), 0, 4095, 0, 255); }
else if(pot1_value == 255 & pot1Dir_value == 1){ potentiometer1Value = map(analogRead(potentiometer1), 0, 4095, 255, 0); }

/*********potentiometer2************/
     if(pot2_value == 180 & pot2Dir_value == 0){ potentiometer2Value = map(analogRead(potentiometer2), 0, 4095, 0, 180); }
else if(pot2_value == 180 & pot2Dir_value == 1){ potentiometer2Value = map(analogRead(potentiometer2), 0, 4095, 180, 0); }
else if(pot2_value == 255 & pot2Dir_value == 0){ potentiometer2Value = map(analogRead(potentiometer2), 0, 4095, 0, 255); }
else if(pot2_value == 255 & pot1Dir_value == 1){ potentiometer2Value = map(analogRead(potentiometer2), 0, 4095, 255, 0); }


/*********joystick1************/
     if(joy1_value == 180 & joy1Dir_value == 0){ joystick1Value = map(analogRead(joystick1), 0, 4095, 0, 180); }
else if(joy1_value == 180 & joy1Dir_value == 1){ joystick1Value = map(analogRead(joystick1), 0, 4095, 180, 0); }
else if(joy1_value == 255 & joy1Dir_value == 0){ joystick1Value = map(analogRead(joystick1), 0, 4095, 0, 255); }
else if(joy1_value == 255 & joy1Dir_value == 1){ joystick1Value = map(analogRead(joystick1), 0, 4095, 255, 0); }

/*********joystick2************/
     if(joy2_value == 180 & joy2Dir_value == 0){ joystick2Value = map(analogRead(joystick2), 0, 4095, 0, 180); }
else if(joy2_value == 180 & joy2Dir_value == 1){ joystick2Value = map(analogRead(joystick2), 0, 4095, 180, 0); }
else if(joy2_value == 255 & joy2Dir_value == 0){ joystick2Value = map(analogRead(joystick2), 0, 4095, 0, 255); }
else if(joy2_value == 255 & joy2Dir_value == 1){ joystick2Value = map(analogRead(joystick2), 0, 4095, 255, 0); }

/*********joystick3************/
     if(joy3_value == 180 & joy3Dir_value == 0){ joystick3Value = map(analogRead(joystick3), 0, 4095, 0, 180); }
else if(joy3_value == 180 & joy3Dir_value == 1){ joystick3Value = map(analogRead(joystick3), 0, 4095, 180, 0); }
else if(joy3_value == 255 & joy3Dir_value == 0){ joystick3Value = map(analogRead(joystick3), 0, 4095, 0, 255); }
else if(joy3_value == 255 & joy3Dir_value == 1){ joystick3Value = map(analogRead(joystick3), 0, 4095, 255, 0); }

/*********joystick4************/
     if(joy4_value == 180 & joy4Dir_value == 0){ joystick4Value = map(analogRead(joystick4), 0, 4095, 0, 180); }
else if(joy4_value == 180 & joy4Dir_value == 1){ joystick4Value = map(analogRead(joystick4), 0, 4095, 180, 0); }
else if(joy4_value == 255 & joy4Dir_value == 0){ joystick4Value = map(analogRead(joystick4), 0, 4095, 0, 255); }
else if(joy4_value == 255 & joy4Dir_value == 1){ joystick4Value = map(analogRead(joystick4), 0, 4095, 255, 0); }


/*********button1*********/
String btn1Type = (btn1Dir_value == 0)?"P":"T";
if(btn1Dir_value == 1){
int button1 = digitalRead(joybutton1);
if(button1 != lastButton1){
  lastButton1 = button1;

  if(lastButton1 == 0){
    button1State = (button1State == 0)?1:0;
  }
}
}
else if(btn1Dir_value == 0){
  button1State = digitalRead(joybutton1);
}
/*********button2*********/
String btn2Type = (btn2Dir_value == 0)?"P":"T";
if(btn2Dir_value == 1){
int button2 = digitalRead(joybutton2);
if(button2 != lastButton2){
  lastButton2 = button2;

  if(lastButton2 == 0){
    button2State = (button2State == 0)?1:0;
  }
}
}
else if(btn2Dir_value == 0){
  button2State = digitalRead(joybutton2);
}
/*********button3*********/
String btn3Type = (btn3Dir_value == 0)?"P":"T";
if(btn3Dir_value == 1){
int button3 = digitalRead(encoder1_btn);
if(button3 != lastButton3){
  lastButton3 = button3;

  if(lastButton3 == 0){
    button3State = (button3State == 0)?1:0;
  }
}
}
else if(btn3Dir_value == 0){
  button3State = digitalRead(encoder1_btn);
}
/*********button4*********/
String btn4Type = (btn4Dir_value == 0)?"P":"T";
if(btn4Dir_value == 1){
int button4 = digitalRead(encoder2_btn);
if(button4 != lastButton4){
  lastButton4 = button4;

  if(lastButton4 == 0){
    button4State = (button4State == 0)?1:0;
  }
}
}
else if(btn4Dir_value == 0){
  button4State = digitalRead(encoder2_btn);
}

/**************roary encoder1*******************/
 // Read the current state of CLK
  currentStateCLK1 = digitalRead(encoder1_CLK);
     if (currentStateCLK1 != lastStateCLK1  && currentStateCLK1 == 1){
        if (digitalRead(encoder1_DT) != currentStateCLK1) {
          encoder1_counter --;
        } else {
          // Encoder is rotating CW so increment
          encoder1_counter ++;
        }
      }
      // Remember last CLK state
      lastStateCLK1 = currentStateCLK1;
      
/**************roary encoder1*******************/
  currentStateCLK2 = digitalRead(encoder2_CLK);
     if (currentStateCLK2 != lastStateCLK2  && currentStateCLK2 == 1){
        if (digitalRead(encoder2_DT) != currentStateCLK2) {
          encoder2_counter --;
        } else {
          // Encoder is rotating CW so increment
          encoder2_counter ++;
        }
      }
       lastStateCLK2 = currentStateCLK2;





if (digitalRead(nextPageButton) == LOW) {
      pagesCount++;
      if (pagesCount == totalPages) { 
        pagesCount = 0;
      }
    }
delay(100);
    


  servo1Dir_Type = (servo1Dir_value == 0)?"KNOB":"SWEEP";
  servo2Dir_Type = (servo2Dir_value == 0)?"KNOB":"SWEEP";
  motor1Dir_Type = (motor1Dir_value == 0)?"CLOCK":"ANTI-CLOCK";
  motor2Dir_Type = (motor2Dir_value == 0)?"CLOCK":"ANTI-CLOCK";
  motor3Dir_Type = (motor3Dir_value == 0)?"CLOCK":"ANTI-CLOCK";
  motor4Dir_Type = (motor4Dir_value == 0)?"CLOCK":"ANTI-CLOCK";
  

 
  switch(pagesCount){
  
    case 0:
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("DASHBOARD");
            display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
            display.setCursor(0,12); display.print("POT1:" + String(potentiometer1Value));display.setCursor(60,12);display.print("| POT2:" + String(potentiometer2Value)); 
            display.setCursor(0,21); display.print("BTN1:" + String(button1State)+"["+ String(btn1Type)+"]"); display.setCursor(60,21);display.print("| BTN2:" + String(button2State)+"["+ String(btn2Type)+"]"); 
            display.setCursor(0,31); display.print("BTN3:" + String(button3State)+"["+ String(btn3Type)+"]"); display.setCursor(60,31);display.print("| BTN4:" + String(button4State)+"["+ String(btn4Type)+"]"); 
            display.setCursor(0,41); display.print("ENC1:" + String(encoder1_counter)); display.setCursor(60,41);display.print("| ENC2:" + String(encoder2_counter)); 
          
          break;
  
    case 1:
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("SERVO");
            display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
            
            display.setCursor(0,12); display.print(" SERVO1 : " + String(servo1Dir_Type));   
            display.setCursor(0,21); display.print(" SERVO2 : " + String(servo2Dir_Type)); 
          break;
          
    case 2:
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("MOTOR");
            display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
            
            display.setCursor(0,12); display.print(" MOTOR1: " + String(motor1Dir_Type));   
            display.setCursor(0,21); display.print(" MOTOR2: " + String(motor2Dir_Type)); 
            display.setCursor(0,31); display.print(" MOTOR3: " + String(motor3Dir_Type));   
            display.setCursor(0,41); display.print(" MOTOR4: " + String(motor4Dir_Type)); 

          break;
    
    case 3:
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("page3");
            display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
//           display.setCursor(0,12); display.print(" POT1:" + String(pot1_value) + "  | "); display.setCursor(70,12);display.print(" BTN1:" + String(btn1Dir_value));   
//           display.setCursor(0,21); display.print(" POT2:" + String(pot2_value) + "  | "); display.setCursor(70,21);display.print(" BTN2:" + String(btn1Dir_value)); 
          break;      
    case 4:
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("page4");
            display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
//           display.setCursor(0,12); display.print(" POT1:" + String(pot1_value) + "  | "); display.setCursor(70,12);display.print(" BTN1:" + String(btn1Dir_value));   
//           display.setCursor(0,21); display.print(" POT2:" + String(pot2_value) + "  | "); display.setCursor(70,21);display.print(" BTN2:" + String(btn1Dir_value)); 
          break;
    case 5:
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("page5");
            display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
//           display.setCursor(0,12); display.print(" POT1:" + String(pot1_value) + "  | "); display.setCursor(70,12);display.print(" BTN1:" + String(btn1Dir_value));   
//           display.setCursor(0,21); display.print(" POT2:" + String(pot2_value) + "  | "); display.setCursor(70,21);display.print(" BTN2:" + String(btn1Dir_value)); 
          break;
    case 6:
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("page6");
            display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
//           display.setCursor(0,12); display.print(" POT1:" + String(pot1_value) + "  | "); display.setCursor(70,12);display.print(" BTN1:" + String(btn1Dir_value));   
//           display.setCursor(0,21); display.print(" POT2:" + String(pot2_value) + "  | "); display.setCursor(70,21);display.print(" BTN2:" + String(btn1Dir_value)); 
          break;                     
  }
           display.display();

 
}

//void UART_TRANSMITTER_DATA(){
//  StaticJsonDocument<500> doc;
//    String SendJSONData;  
//
//    JsonArray button = doc.createNestedArray("buttonStatus");
//    button.add(btn1Dir_value);
//    button.add(btn2Dir_value);
//    button.add(btn3Dir_value);
//    button.add(btn4Dir_value);
//            
//    JsonArray potentiometers = doc.createNestedArray("potStatus");
//    potentiometers.add(pot1_value);
//    potentiometers.add(pot2_value);
//    potentiometers.add(pot3_value);
//    potentiometers.add(pot4_value);
//
//                
//    serializeJson(doc, SendJSONData);
////    Serial.println(SendJSONData);
//    Serial2.println(SendJSONData);
////    Serial.print(SendJSONData);
////    TempSerial.write(SendJSONData);
//
//}


void sendDataToSlave(){

String SendJSONData = "";
StaticJsonDocument<1024> doc;

   JsonArray button = doc.createNestedArray("button");
    button.add(button1State);
    button.add(button2State);
    button.add(button3State);
    button.add(button4State);
            
    JsonArray potentiometers = doc.createNestedArray("potentiometers");
    potentiometers.add(potentiometer1Value);
    potentiometers.add(potentiometer2Value);


    JsonArray joysticks = doc.createNestedArray("joysticks");
    joysticks.add(joystick1Value);
    joysticks.add(joystick2Value);
    joysticks.add(joystick3Value);
    joysticks.add(joystick4Value);  
    

serializeJson(doc, SendJSONData);
//serializeJsonPretty(doc, SendJSONData);



  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime > interval) { //sending every 1 second
    lastSendTime = currentMillis; 
  Serial.println("Sending " + SendJSONData);
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(SendJSONData.length());        // add payload length
  LoRa.print(SendJSONData);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
  }  

}

void UART_RECEIVER_DATA(){
   if (Serial2.available() > 0) {
    String UART_incomingData = Serial2.readString();
    Serial.print("UART_incomingData: ");
    Serial.println(UART_incomingData);

    StaticJsonDocument<1024> UART_RX;
    DeserializationError error = deserializeJson(UART_RX, UART_incomingData);

    int btn1 = UART_RX["button"][0];
    int btn2 = UART_RX["button"][1];
    int btn3 = UART_RX["button"][2];
    int btn4 = UART_RX["button"][3];
    
    int pot1 = UART_RX["potentiometers"][0];
    int pot1Dir = UART_RX["potentiometers"][1];
    
    int pot2 = UART_RX["potentiometers"][2];
    int pot2Dir = UART_RX["potentiometers"][3];
    int pot3 = UART_RX["potentiometers"][4];
    int pot3Dir = UART_RX["potentiometers"][5];
    int pot4 = UART_RX["potentiometers"][6];
    int pot4Dir = UART_RX["potentiometers"][7];
    
    int joy1 = UART_RX["joysticks"][0];
    int joy1Dir = UART_RX["joysticks"][1];
    int joy2 = UART_RX["joysticks"][2];
    int joy2Dir = UART_RX["joysticks"][3];
    int joy3 = UART_RX["joysticks"][4];
    int joy3Dir = UART_RX["joysticks"][5];
    int joy4 = UART_RX["joysticks"][6];
    int joy4Dir = UART_RX["joysticks"][7];

    int motor1 = UART_RX["motors"][0];
    int motor2 = UART_RX["motors"][1];
    int motor3 = UART_RX["motors"][2];
    int motor4 = UART_RX["motors"][3];

    int servo1 = UART_RX["servos"][0];
    int servo2 = UART_RX["servos"][1];
                        
    EEPROM.write(btn1Dir_address, btn1);
    EEPROM.write(btn2Dir_address, btn2);
    EEPROM.write(btn3Dir_address, btn3);
    EEPROM.write(btn4Dir_address, btn4);
    EEPROM.write(pot1_address, pot1);
    EEPROM.write(pot2_address, pot2);
    EEPROM.write(pot3_address, pot3);
    EEPROM.write(pot4_address, pot4);
    EEPROM.write(pot1Dir_address, pot1Dir);  
    EEPROM.write(pot2Dir_address, pot2Dir);    
    EEPROM.write(pot3Dir_address, pot3Dir);
    EEPROM.write(pot4Dir_address, pot4Dir); 
    EEPROM.write(joy1_address, joy1);
    EEPROM.write(joy2_address, joy2);
    EEPROM.write(joy3_address, joy3);
    EEPROM.write(joy4_address, joy4);
    EEPROM.write(joy1Dir_address, joy1Dir);
    EEPROM.write(joy2Dir_address, joy2Dir);
    EEPROM.write(joy3Dir_address, joy3Dir);
    EEPROM.write(joy4Dir_address, joy4Dir);
    EEPROM.write(motor1Dir_address, motor1);
    EEPROM.write(motor2Dir_address, motor2);
    EEPROM.write(motor3Dir_address, motor3);
    EEPROM.write(motor4Dir_address, motor4);
    EEPROM.write(servo1Dir_address, servo1);
    EEPROM.write(servo2Dir_address, servo2);
       
    EEPROM.commit();

delay(1000);
resetFunc();  //call reset

//    Serial.println("btn1:" + String(btn1));
  }
}
