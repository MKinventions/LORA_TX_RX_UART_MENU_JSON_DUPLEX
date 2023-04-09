#include<EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Wire_slave.h>
#include <ArduinoJson.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const int nextPageButton = 18;
const int previouPageButton = 15;
const int MainMenuButton = 23;
const int SubMenuButton = 5;
const int SaveAndRestart = 4;
const int Buzzer = 19;


int pagesCount = 0;
int MainMenuSelect = 0;
int SubMenuSelect = 0;

int totalPages = 6;
int totalMenus = 10;
int totalSubMenus = 4;


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

int pageCount_address = 17;

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


String pot1Dir_Type, pot2Dir_Type, pot3Dir_Type, pot4Dir_Type;
String joy1Dir_Type, joy2Dir_Type, joy3Dir_Type, joy4Dir_Type;
String btn1Dir_Type, btn2Dir_Type, btn3Dir_Type, btn4Dir_Type;
String servo1Dir_Type, servo2Dir_Type;
String motor1Dir_Type, motor2Dir_Type, motor3Dir_Type, motor4Dir_Type;


/***********voltage sensor start**********************/
const int voltSensor = 27;
float voltageOUT = 0.0;
float voltageIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int voltageSensor = 0;
float inputVoltage = 3.3;//5v
float voltagePercentage;
/***********voltage sensor end**********************/

void(* resetFunc) (void) = 0; //declare reset function @ address 0

//UART PINS
#define RXp2 16
#define TXp2 17
unsigned long lastMillisTx = 0;
unsigned long lastMillisRx = 0;
int btn1State;
int btn2State;
int btn3State;
int btn4State;
int pot1State;
int pot2State;
int pot3State;
int pot4State;

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  EEPROM.begin(255);
  pinMode(nextPageButton, INPUT_PULLUP);
  pinMode(previouPageButton, INPUT_PULLUP);
  pinMode(MainMenuButton, INPUT_PULLUP);
  pinMode(SubMenuButton, INPUT_PULLUP);
  pinMode(SaveAndRestart, INPUT_PULLUP);
  pinMode(Buzzer, OUTPUT);
  pinMode(voltSensor, INPUT);

  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//  display.clearDisplay();
//  display.setTextColor(WHITE);
//  display.setTextSize(1);
//  display.setCursor(40, 30);
//  display.println("RESTARTED");
//  display.display();
  
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

  pagesCount = EEPROM.read(pageCount_address);//EEPROM.read(ADDRESS 0-255); 

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

 
   StaticJsonDocument<500> doc;
    String SendJSONData;  
//    doc["mac"] = "asdf545";
//    doc["port"] = "8080";

    JsonArray button = doc.createNestedArray("button");
    button.add(btn1Dir_value);
    button.add(btn2Dir_value);
    button.add(btn3Dir_value);
    button.add(btn4Dir_value);
            
    JsonArray potentiometers = doc.createNestedArray("potentiometers");
    potentiometers.add(pot1_value);
    potentiometers.add(pot1Dir_value);
    potentiometers.add(pot2_value);
    potentiometers.add(pot2Dir_value);
    potentiometers.add(pot3_value);
    potentiometers.add(pot3Dir_value);
    potentiometers.add(pot4_value);
    potentiometers.add(pot4Dir_value);

    JsonArray joysticks = doc.createNestedArray("joysticks");
    joysticks.add(joy1_value);
    joysticks.add(joy1Dir_value);
    joysticks.add(joy2_value);
    joysticks.add(joy2Dir_value);
    joysticks.add(joy3_value);
    joysticks.add(joy3Dir_value);    
    joysticks.add(joy4_value);
    joysticks.add(joy4Dir_value);    

    JsonArray motors = doc.createNestedArray("motors");
    motors.add(motor1Dir_value);
    motors.add(motor2Dir_value);
    motors.add(motor3Dir_value);
    motors.add(motor4Dir_value);

    JsonArray servos = doc.createNestedArray("servos");
    servos.add(servo1Dir_value);
    servos.add(servo2Dir_value);
            
    serializeJson(doc, SendJSONData);
    Serial.println(SendJSONData);
    Serial2.println(SendJSONData);
//    Serial.print(SendJSONData);
//    TempSerial.write(SendJSONData);

 }

void loop() 
{


//UART_RECEIVER_DATA();

/***********voltage sensor start**********************/
  voltageSensor = analogRead(voltSensor);
//  voltageOUT = (voltageSensor * 5.0) / 1024.0;
  voltageOUT = (voltageSensor * inputVoltage) / 4095.0;
  voltageIN = voltageOUT / (R2/(R1+R2));
  voltagePercentage = voltageOUT  * 100;
/***********voltage sensor end**********************/


  

unsigned long uart_tx_millis = millis();

if(uart_tx_millis - lastMillisTx >= 100){  
   lastMillisTx = uart_tx_millis;
   

    if (digitalRead(nextPageButton) == LOW) {
      pagesCount++;
      if (pagesCount >= totalPages) { 
        pagesCount = totalPages;
      }
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
    }

    if (digitalRead(previouPageButton) == LOW) {
      pagesCount--;
      if (pagesCount <= 0) {
        pagesCount = 0;
      }
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
    }
   
    if (digitalRead(MainMenuButton) == LOW) {
      MainMenuSelect++;
      if (MainMenuSelect == totalMenus+1) {
        MainMenuSelect = 0;
      }
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
   }
    
    if (digitalRead(SubMenuButton) == LOW) {
      SubMenuSelect++;
      if (SubMenuSelect == totalSubMenus+1) {
        SubMenuSelect = 0;
      }
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
    }

    if (digitalRead(SaveAndRestart) == LOW) {  
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
        delay(100);
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
       resetFunc();  //call reset
 
     }else{
    digitalWrite(Buzzer, LOW);
     }
    
  
  pot1Dir_Type = (pot1Dir_value == 0)?"LFT":"RHT";
  pot2Dir_Type = (pot2Dir_value == 0)?"LFT":"RHT";
  pot3Dir_Type = (pot3Dir_value == 0)?"LFT":"RHT";
  pot4Dir_Type = (pot4Dir_value == 0)?"LFT":"RHT";
  joy1Dir_Type = (joy1Dir_value == 0)?"LFT":"RHT";
  joy2Dir_Type = (joy2Dir_value == 0)?"LFT":"RHT";
  joy3Dir_Type = (joy3Dir_value == 0)?"LFT":"RHT";
  joy4Dir_Type = (joy4Dir_value == 0)?"LFT":"RHT";
  btn1Dir_Type = (btn1Dir_value == 0)?"PUSH":"TOGGLE";
  btn2Dir_Type = (btn2Dir_value == 0)?"PUSH":"TOGGLE";
  btn3Dir_Type = (btn3Dir_value == 0)?"PUSH":"TOGGLE";
  btn4Dir_Type = (btn4Dir_value == 0)?"PUSH":"TOGGLE";
  servo1Dir_Type = (servo1Dir_value == 0)?"KNOB":"SWEEP";
  servo2Dir_Type = (servo2Dir_value == 0)?"KNOB":"SWEEP";
  motor1Dir_Type = (motor1Dir_value == 0)?"CLOCK":"ANTI-CLOCK";
  motor2Dir_Type = (motor2Dir_value == 0)?"CLOCK":"ANTI-CLOCK";
  motor3Dir_Type = (motor3Dir_value == 0)?"CLOCK":"ANTI-CLOCK";
  motor4Dir_Type = (motor4Dir_value == 0)?"CLOCK":"ANTI-CLOCK";

  
 
  
  switch(pagesCount){
  
    case 0:
            EEPROM.write(pageCount_address, 0);
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(0, 0);//COL,ROW
            display.println("DASHBOARD");
            battery_voltage();
            display.setCursor(0,12); display.print("BTRY:" + String(voltageIN) + "V (" + String(voltagePercentage)+" %)");   
            display.setCursor(0,23); display.print("POT1:" + String(pot1State) + "  | "); display.setCursor(60,23);display.print(" BTN1:" + String(btn1State));   
            display.setCursor(0,36); display.print("POT2:" + String(pot2State) + "  | "); display.setCursor(60,36);display.print(" BTN2:" + String(btn2State)); 

            display.display();
          break;
  
    case 1:
          EEPROM.write(pageCount_address, 1);
          potentiometer();   
          break;
          
    case 2:
          EEPROM.write(pageCount_address, 2);
          potentiometer_settings();
          break;
    
    case 3:
          EEPROM.write(pageCount_address, 3);
          button_settings();
          break;      
    case 4:
          EEPROM.write(pageCount_address, 4);
          servo_settings();
          break;
    case 5:
          EEPROM.write(pageCount_address, 5);
          motor_settings();
          break;
    case 6:
          EEPROM.write(pageCount_address, 6);
          settings();
          break;                     
  }
  EEPROM.commit(); 
}
}

void battery_voltage(){

if(voltagePercentage > 75){
      display.setCursor(65,0);display.print("[|||]% " + String(pagesCount) + "/" + String(totalPages));
}
else if(voltagePercentage > 50 || voltagePercentage < 75 ){
      display.setCursor(65,0);display.print("[|| ]% " + String(pagesCount) + "/" + String(totalPages));
}
else if(voltagePercentage > 25 || voltagePercentage < 50 ){
      display.setCursor(65,0);display.print("[|  ]% " + String(pagesCount) + "/" + String(totalPages));
}
else if(voltagePercentage > 10 || voltagePercentage < 25 ){
      display.setCursor(65,0);display.print("[   ]% " + String(pagesCount) + "/" + String(totalPages));
}
}


//void UART_RECEIVER_DATA(){
//
//unsigned long uart_rx_millis = millis();
//
//if(uart_rx_millis - lastMillisRx >= 10){  
//   lastMillisRx = uart_rx_millis;
//
//    String UART_incomingData = Serial2.readString();
////    Serial.print("UART_incomingData: ");
////    Serial.println(UART_incomingData);
//
//    StaticJsonDocument<1024> UART_TX;
//    DeserializationError error = deserializeJson(UART_TX, UART_incomingData);
//
//    btn1State = UART_TX["buttonStatus"][0];
//    btn2State = UART_TX["buttonStatus"][1];
//    btn3State = UART_TX["buttonStatus"][2];
//    btn4State = UART_TX["buttonStatus"][3];
//    
//    pot1State = UART_TX["potStatus"][0];   
//    pot2State = UART_TX["potStatus"][1];
//    pot3State = UART_TX["potStatus"][2];
//    pot4State = UART_TX["potStatus"][3];
//  Serial.println("Read TX =  btn1:" + String(btn1State)+", btn2:" + String(btn1State));
//  Serial.println("Read TX =  pot1Level:" + String(pot1State)+", pot2Level:" + String(pot2State));
//}
//}


void potentiometer(){
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(0, 0);//COL,ROW
          display.println("POTENTIOMETER");
          display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));

          display.setCursor(0,12); display.print(" POT1:" + String(pot1_value) + "  | "); display.setCursor(70,12);display.print(" JOY1:" + String(joy1_value));   
          display.setCursor(0,21); display.print(" POT2:" + String(pot2_value) + "  | "); display.setCursor(70,21);display.print(" JOY2:" + String(joy2_value)); 
          display.setCursor(0,31); display.print(" POT3:" + String(pot3_value) + "  | "); display.setCursor(70,31);display.print(" JOY3:" + String(joy3_value)); 
          display.setCursor(0,41); display.print(" POT4:" + String(pot4_value) + "  | "); display.setCursor(70,41);display.print(" JOY4:" + String(joy4_value)); 

            switch(MainMenuSelect){
            case 0:
                  display.setCursor(0,55);display.print("Press To Select Menu");
                  break;       
            case 1:
                  display.setCursor(0,12); display.print(">POT1:" + String(pot1_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(pot1_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(pot1_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;                   
            case 2:
                  display.setCursor(0,21); display.print(">POT2:" + String(pot2_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(pot2_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(pot2_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;  
            case 3:
                  display.setCursor(0,31); display.print(">POT3:" + String(pot3_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(pot3_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(pot3_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;  
            case 4:
                  display.setCursor(0,41); display.print(">POT4:" + String(pot4_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(pot4_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(pot4_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;  


            case 5:
                  display.setCursor(70,12); display.print(">JOY1:" + String(joy1_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(joy1_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(joy1_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;                   

            case 6:
                  display.setCursor(70,21); display.print(">JOY2:" + String(joy2_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(joy2_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(joy2_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;   
            case 7:
                  display.setCursor(70,31); display.print(">JOY3:" + String(joy3_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(joy3_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(joy3_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;   
            case 8:
                  display.setCursor(70,41); display.print(">JOY4:" + String(joy4_value));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(30,55);display.print("  180  255");
                                break;
                         case 1:
                                EEPROM.write(joy4_address, 180);
                                display.setCursor(30,55);display.print(" >180  255");
                                break;
                         case 2:
                                EEPROM.write(joy4_address, 255);
                                display.setCursor(30,55);display.print("  180 >255");
                                break;     
                  }  
                  break;                                 
            case 9:
                  MainMenuSelect = 0;
                  break;                         
                  }
        display.display(); 
     
}



void potentiometer_settings(){
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(0, 0);//COL,ROW
          display.println("POT SETTINGS");
          display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));

          display.setCursor(0,12); display.print(" POT1:" + String(pot1Dir_Type)); display.setCursor(70,12);display.print(" JOY1:" + String(joy1Dir_Type));   
          display.setCursor(0,21); display.print(" POT2:" + String(pot2Dir_Type)); display.setCursor(70,21);display.print(" JOY2:" + String(joy2Dir_Type)); 
          display.setCursor(0,31); display.print(" POT3:" + String(pot3Dir_Type)); display.setCursor(70,31);display.print(" JOY3:" + String(joy3Dir_Type)); 
          display.setCursor(0,41); display.print(" POT4:" + String(pot4Dir_Type)); display.setCursor(70,41);display.print(" JOY4:" + String(joy4Dir_Type)); 

            switch(MainMenuSelect){
            case 0:
                  display.setCursor(0,55);display.print("Press To Select Menu");
                  break;       
            case 1:
                  display.setCursor(0,12); display.print(">POT1:" + String(pot1Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(pot1Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(pot1Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;                   
            case 2:
                  display.setCursor(0,21); display.print(">POT2:" + String(pot2Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(pot2Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(pot2Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;    
            case 3:
                  display.setCursor(0,31); display.print(">POT3:" + String(pot3Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(pot3Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(pot3Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;   
            case 4:
                  display.setCursor(0,41); display.print(">POT4:" + String(pot4Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(pot4Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(pot4Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;   
            case 5:
                  display.setCursor(70,12); display.print(">JOY1:" + String(joy1Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(joy1Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(joy1Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;   
            case 6:
                  display.setCursor(70,21); display.print(">JOY2:" + String(joy2Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(joy2Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(joy2Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;  
            case 7:
                  display.setCursor(70,31); display.print(">JOY3:" + String(joy3Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(joy3Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(joy3Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;  
            case 8:
                  display.setCursor(70,41); display.print(">JOY4:" + String(joy4Dir_Type));
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(20,55);display.print("  LEFT  RIGHT");
                                break;
                         case 1:
                                EEPROM.write(joy4Dir_address, 0);
                                display.setCursor(20,55);display.print(" >LEFT  RIGHT");
                                break;
                         case 2:
                                EEPROM.write(joy4Dir_address, 1);
                                display.setCursor(20,55);display.print("  LEFT >RIGHT");
                                break;     
                  }  
                  break;  
                                                                              
            case 9:
                  MainMenuSelect = 0;
                  break;                         
                  }
        display.display(); 
     
}


void button_settings(){
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(0, 0);//COL,ROW
          display.println("BUTTON");
          display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
          
          display.setCursor(0,12);display.print(" BUTTON1 : " + String(btn1Dir_Type));
          display.setCursor(0,21);display.print(" BUTTON2 : " + String(btn2Dir_Type));   
          display.setCursor(0,31);display.print(" BUTTON3 : " + String(btn3Dir_Type));
          display.setCursor(0,41);display.print(" BUTTON4 : " + String(btn4Dir_Type)); 

           switch(MainMenuSelect){
            case 0:
                  display.setCursor(0,55);display.print("Select Button Type");
                  break;          
              
          case 1:
                 display.setCursor(0,12); display.print(">BUTTON1 : " + String(btn1Dir_Type));
                 switch(SubMenuSelect){
                     case 0:
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 1:
                            EEPROM.write(btn1Dir_address, 0);
                            display.setCursor(10,55);display.print(" >PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 2:
                            EEPROM.write(btn1Dir_address, 1);
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print(" >TOGGLE");
                            break;
                     case 3:
                           SubMenuSelect = 0;
                           break;           
              }  
              break;                   
        case 2:
              display.setCursor(0,21); display.print(">BUTTON2 : " + String(btn2Dir_Type));
              switch(SubMenuSelect){
                     case 0:
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 1:
                            EEPROM.write(btn2Dir_address, 0);
                            display.setCursor(10,55);display.print(" >PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 2:
                            EEPROM.write(btn2Dir_address, 1);
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print(" >TOGGLE");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break; 
              }   
              break;  
        case 3:
              display.setCursor(0,31); display.print(">BUTTON3 : " + String(btn3Dir_Type));
              switch(SubMenuSelect){
                     case 0:
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 1:
                            EEPROM.write(btn3Dir_address, 0);
                            display.setCursor(10,55);display.print(" >PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 2:
                            EEPROM.write(btn3Dir_address, 1);
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print(" >TOGGLE");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break; 
              }  
              break;                    
        case 4:
              display.setCursor(0,41); display.print(">BUTTON4 : " + String(btn4Dir_Type));
              switch(SubMenuSelect){
                     case 0:
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 1:
                            EEPROM.write(btn4Dir_address, 0);
                            display.setCursor(10,55);display.print(" >PUSH");display.setCursor(60,55);display.print("  TOGGLE");
                            break;
                     case 2:
                            EEPROM.write(btn4Dir_address, 1);
                            display.setCursor(10,55);display.print("  PUSH");display.setCursor(60,55);display.print(" >TOGGLE");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break; 
              }   
              break;                    
                       
         case 5:
                  MainMenuSelect = 0;
                  break;   
              } 
        display.display();        
}


void servo_settings(){
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(0, 0);//COL,ROW
          display.println("SERVO");
          display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));
          
          display.setCursor(0,12); display.print(" SERVO1 : " + String(servo1Dir_Type));   
          display.setCursor(0,21); display.print(" SERVO2 : " + String(servo2Dir_Type)); 

           switch(MainMenuSelect){
            case 0:
                  display.setCursor(0,55);display.print("Select Servo Type");
                  break;          
              
          case 1:
                 display.setCursor(0,12); display.print(">SERVO1 : " + String(servo1Dir_Type));
                 switch(SubMenuSelect){
                     case 0:
                            display.setCursor(10,55);display.print("  KNOB   SWEEP");
                            break;
                     case 1:
                            EEPROM.write(servo1Dir_address, 0);
                            display.setCursor(10,55);display.print(" >KNOB   SWEEP");
                            break;
                     case 2:
                            EEPROM.write(servo1Dir_address, 1);
                            display.setCursor(10,55);display.print("  KNOB  >SWEEP");
                            break;
                     case 3:
                           SubMenuSelect = 0;
                           break;                                 
              }  
              break;                   
          case 2:
                 display.setCursor(0,21); display.print(">SERVO2 : " + String(servo2Dir_Type));
                 switch(SubMenuSelect){
                     case 0:
                            display.setCursor(10,55);display.print("  KNOB   SWEEP");
                            break;
                     case 1:
                            EEPROM.write(servo2Dir_address, 0);
                            display.setCursor(10,55);display.print(" >KNOB   SWEEP");
                            break;
                     case 2:
                            EEPROM.write(servo2Dir_address, 1);
                            display.setCursor(10,55);display.print("  KNOB  >SWEEP");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break; 
                 }  
              break;  
                
                       
         case 3:
                  MainMenuSelect = 0;
                  break;   
              }  
       display.display();        
}

void motor_settings(){
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
          
           switch(MainMenuSelect){
            case 0:
                  display.setCursor(0,55);display.print("Select Motor Direct..");
                  break;          
              
          case 1:
                 display.setCursor(0,12); display.print(">MOTOR1: " + String(motor1Dir_Type));
                 switch(SubMenuSelect){
                     case 0:
                            display.setCursor(0,55);display.print("  CLOCK   ANTI-CLOCK");
                            break;
                     case 1:
                            EEPROM.write(motor1Dir_address, 0);
                            display.setCursor(0,55);display.print(" >CLOCK   ANTI-CLOCK");
                            break;
                     case 2:
                            EEPROM.write(motor1Dir_address, 1);
                            display.setCursor(0,55);display.print("  CLOCK  >ANTI-CLOCK");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break; 
                   }  
              break; 
                                
          case 2:
                 display.setCursor(0,21); display.print(">MOTOR2: " + String(motor2Dir_Type));
                 switch(SubMenuSelect){
                     case 0:
                            display.setCursor(0,55);display.print("  CLOCK   ANTI-CLOCK");
                            break;
                     case 1:
                            EEPROM.write(motor2Dir_address, 0);
                            display.setCursor(0,55);display.print(" >CLOCK   ANTI-CLOCK");
                            break;
                     case 2:
                            EEPROM.write(motor2Dir_address, 1);
                            display.setCursor(0,55);display.print("  CLOCK  >ANTI-CLOCK");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break; 
                 }  
              break;  
                
            case 3:
                 display.setCursor(0,31); display.print(">MOTOR3: " + String(motor3Dir_Type));
                 switch(SubMenuSelect){
                     case 0:
                            display.setCursor(0,55);display.print("  CLOCK   ANTI-CLOCK");
                            break;
                     case 1:
                            EEPROM.write(motor3Dir_address, 0);
                            display.setCursor(0,55);display.print(" >CLOCK   ANTI-CLOCK");
                            break;
                     case 2:
                            EEPROM.write(motor3Dir_address, 1);
                            display.setCursor(0,55);display.print("  CLOCK  >ANTI-CLOCK");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break;    
                      }  
              break;  
              
          case 4:
                 display.setCursor(0,41); display.print(">MOTOR4: " + String(motor4Dir_Type));
                 switch(SubMenuSelect){
                     case 0:
                            display.setCursor(0,55);display.print("  CLOCK   ANTI-CLOCK");
                            break;
                     case 1:
                            EEPROM.write(motor4Dir_address, 0);
                            display.setCursor(0,55);display.print(" >CLOCK   ANTI-CLOCK");
                            break;
                     case 2:
                            EEPROM.write(motor4Dir_address, 1);
                            display.setCursor(0,55);display.print("  CLOCK  >ANTI-CLOCK");
                            break;     
                     case 3:
                           SubMenuSelect = 0;
                           break; 
                   }  
              break;
                                 
         case 5:
                  MainMenuSelect = 0;
                  break;   
              }  
       display.display();        
}

void settings(){

          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(0, 0);//COL,ROW
          display.println("SETTINGS");
          display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));

          

          
         switch(MainMenuSelect){
            case 0:
                  display.setCursor(0,12);display.print(" Factory Reset");
                  display.setCursor(0,22);display.print(" menu1"); 
                  display.setCursor(0,32);display.print(" menu2"); 
                  display.display();
                  break;       
            case 1:
                  display.setCursor(0,12);display.print(">Factory Reset");
                  display.setCursor(0,22);display.print(" menu1"); 
                  display.setCursor(0,32);display.print(" menu2");   
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
                                display.display(); 
                                break;
                         case 1:
//                               EEPROM.write(pot1_address, 180);
//                               EEPROM.write(pot2_address, 180);
//                               EEPROM.write(pot3_address, 180);
//                               EEPROM.write(pot4_address, 180);
//                               EEPROM.write(pot1Dir_address, 1);
//                               EEPROM.write(pot2Dir_address, 1);
//                               EEPROM.write(pot3Dir_address, 0);
//                               EEPROM.write(pot4Dir_address, 0);
//                               EEPROM.write(joy1_address, 255);
//                               EEPROM.write(joy2_address, 255);
//                               EEPROM.write(joy3_address, 255);
//                               EEPROM.write(joy4_address, 255);
//                               EEPROM.write(joy1Dir_address, 1);
//                               EEPROM.write(joy2Dir_address, 1);
//                               EEPROM.write(joy3Dir_address, 0);
//                               EEPROM.write(joy4Dir_address, 0);
//                               EEPROM.write(btn1Dir_address, 0);
//                               EEPROM.write(btn2Dir_address, 0);
//                               EEPROM.write(btn3Dir_address, 0);
//                               EEPROM.write(btn4Dir_address, 0);
//                               EEPROM.write(motor1Dir_address, 0);
//                               EEPROM.write(motor2Dir_address, 1);
//                               EEPROM.write(motor3Dir_address, 0);
//                               EEPROM.write(motor4Dir_address, 1);
                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
                                display.display(); 
                                break;
                         case 2:
//                               EEPROM.write(pot1_address, pot1_value);
//                               EEPROM.write(pot2_address, pot2_value);
//                               EEPROM.write(pot3_address, pot3_value);
//                               EEPROM.write(pot4_address, pot4_value);
//                               EEPROM.write(pot1Dir_address, pot1Dir_value);
//                               EEPROM.write(pot2Dir_address, pot2Dir_value);
//                               EEPROM.write(pot3Dir_address, pot3Dir_value);
//                               EEPROM.write(pot4Dir_address, pot4Dir_value);
//                               EEPROM.write(joy1_address, joy1_value);
//                               EEPROM.write(joy2_address, joy2_value);
//                               EEPROM.write(joy3_address, joy3_value);
//                               EEPROM.write(joy4_address, joy4_value);
//                               EEPROM.write(joy1Dir_address, joy1Dir_value);
//                               EEPROM.write(joy2Dir_address, joy2Dir_value);
//                               EEPROM.write(joy3Dir_address, joy3Dir_value);
//                               EEPROM.write(joy4Dir_address, joy4Dir_value);
//                               
//                               EEPROM.write(btn1Dir_address, btn1Dir_value);
//                               EEPROM.write(btn2Dir_address, btn2Dir_value);
//                               EEPROM.write(btn3Dir_address, btn3Dir_value);
//                               EEPROM.write(btn4Dir_address, btn4Dir_value);
//
//                               EEPROM.write(motor1Dir_address, motor1Dir_value);
//                               EEPROM.write(motor2Dir_address, motor2Dir_value);
//                               EEPROM.write(motor3Dir_address, motor3Dir_value);
//                               EEPROM.write(motor4Dir_address, motor4Dir_value);
                               
                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
                                display.display(); 
                                break;     
                  }  
                  break;                   
            case 2:
                  display.setCursor(0,12);display.print(" Factory Reset");
                  display.setCursor(0,22);display.print(">menu1"); 
                  display.setCursor(0,32);display.print(" menu2");                  
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
                                display.display(); 
                                break;
                         case 1:
                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
                                display.display(); 
                                break;
                         case 2:
                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
                                display.display(); 
                                break;     
                  }  
                  break;  
            case 3:
                  display.setCursor(0,12);display.print(" Factory Reset");
                  display.setCursor(0,22);display.print(" menu1"); 
                  display.setCursor(0,32);display.print(">menu2");                  
                  switch(SubMenuSelect){
                         case 0:
                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
                                display.display(); 
                                break;
                         case 1:
                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
                                display.display(); 
                                break;
                         case 2:
                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
                                display.display(); 
                                break;     
                  }  
                  break;  
                  }        
            
               
}



void deviceDetails(){

          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(0, 0);//COL,ROW
          display.println("DEVICE DETAILS");
          display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));

          

          
         switch(MainMenuSelect){
            case 0:
                  display.setCursor(0,12);display.print(" MAC-ID");
                  display.setCursor(0,22);display.print(" menu1"); 
                  display.setCursor(0,32);display.print(" menu2"); 
                  
                  break;       
            case 1:
                  display.setCursor(0,12);display.print(">MAC-ID");
                  display.setCursor(0,22);display.print(" menu1"); 
                  display.setCursor(0,32);display.print(" menu2");  
                  display.setCursor(10,55);display.print("mac addresss"); 
//                  switch(SubMenuSelect){
//                         case 0:
//                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
//                                display.display(); 
//                                break;
//                         case 1:
//                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
//                                display.display(); 
//                                break;
//                         case 2:
//                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
//                                display.display(); 
//                                break;     
//                  }  
                  break;                   
            case 2:
                  display.setCursor(0,12);display.print(" MAC-ID");
                  display.setCursor(0,22);display.print(">menu1"); 
                  display.setCursor(0,32);display.print(" menu2");                  
//                  switch(SubMenuSelect){
//                         case 0:
//                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
//                                display.display(); 
//                                break;
//                         case 1:
//                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
//                                display.display(); 
//                                break;
//                         case 2:
//                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
//                                display.display(); 
//                                break;     
//                  }  
                  break;  
            case 3:
                  display.setCursor(0,12);display.print(" MAC-ID");
                  display.setCursor(0,22);display.print(" menu1"); 
                  display.setCursor(0,32);display.print(">menu2");  
                                  
//                  switch(SubMenuSelect){
//                         case 0:
//                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
//                                display.display(); 
//                                break;
//                         case 1:
//                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
//                                display.display(); 
//                                break;
//                         case 2:
//                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
//                                display.display(); 
//                                break;     
//                  }  
                  break;  
                  }        
            
           display.display();    
}

void slaveDeviceDetails(){

          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(0, 0);//COL,ROW
          display.println("SLAVE DETAILS");
          display.setCursor(110,0);display.print(String(pagesCount) + "/" + String(totalPages));

          
           display.setCursor(0,12);display.print("MAC : ");display.print("EC62609B1F14");
           display.setCursor(0,22);display.print("Batt: ");display.print("3.3");display.print(" V");
           display.setCursor(0,32);display.print("RSSI: ");display.print("50");display.print(" dBm"); 
           display.setCursor(0,42);display.print("Temp: ");display.print("35");
           display.setCursor(0,52);display.print("Acce: ");display.print("35");

          
//         switch(MainMenuSelect){
//            case 0:
//                  display.setCursor(0,12);display.print(" MAC-ID");
//                  display.setCursor(0,22);display.print(" menu1"); 
//                  display.setCursor(0,32);display.print(" menu2"); 
//                  
//                  break;       
//            case 1:
//                  display.setCursor(0,12);display.print(">MAC-ID");
//                  display.setCursor(0,22);display.print(" menu1"); 
//                  display.setCursor(0,32);display.print(" menu2");  
//                  display.setCursor(10,55);display.print(WiFi.macAddress()); 
////                  switch(SubMenuSelect){
////                         case 0:
////                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
////                                display.display(); 
////                                break;
////                         case 1:
////                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
////                                display.display(); 
////                                break;
////                         case 2:
////                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
////                                display.display(); 
////                                break;     
////                  }  
//                  break;                   
//            case 2:
//                  display.setCursor(0,12);display.print(" MAC-ID");
//                  display.setCursor(0,22);display.print(">menu1"); 
//                  display.setCursor(0,32);display.print(" menu2");                  
////                  switch(SubMenuSelect){
////                         case 0:
////                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
////                                display.display(); 
////                                break;
////                         case 1:
////                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
////                                display.display(); 
////                                break;
////                         case 2:
////                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
////                                display.display(); 
////                                break;     
////                  }  
//                  break;  
//            case 3:
//                  display.setCursor(0,12);display.print(" MAC-ID");
//                  display.setCursor(0,22);display.print(" menu1"); 
//                  display.setCursor(0,32);display.print(">menu2");                  
////                  switch(SubMenuSelect){
////                         case 0:
////                                display.setCursor(10,55);display.print("  [YES] ");display.print("   [NO]");
////                                display.display(); 
////                                break;
////                         case 1:
////                                display.setCursor(10,55);display.print("=>[YES] ");display.print("   [NO]");
////                                display.display(); 
////                                break;
////                         case 2:
////                                display.setCursor(10,55);display.print("  [YES] ");display.print(" =>[NO]");
////                                display.display(); 
////                                break;     
////                  }  
//                  break;  
//                  }        
            
           display.display();    
}
