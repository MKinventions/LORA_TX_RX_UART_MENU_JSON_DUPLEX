# LORA_TX_RX_UART_MENU_JSON_DUPLEX






nextPageButton    => 18
previouPageButton => 15
MainMenuButton    => 23
SubMenuButton     => 5
SaveAndRestart    => 4
Buzzer            => 19






//MASTER

RX2--> UARTTX

TX2--> UARTRX


D2--->LORA-DIO0

D5--->LORA-NSS/CS

D18-->LORA-SCK

D19-->LORA-MISO

D23-->LORA-MOSI

D14-->LORA-RST



D21-->SDA/OLED

D22-->SCL/OLED


RX0/gpio3-->encoder2_btn

TX0/gpio1--> pagecountbutton


D15-->encoder2_DT

D4--->encoder2_CLK

D33-->encoder1_DT

D32-->encoder1_CLK

D25-->encoder1_btn



D27-->POT2

D26-->POT1


D35(ADC1_7)-->JOY1

D34(ADC1_6)-->JOY2

VN(GPIO39/ADC1_0)-->JOY3

VP(GPIO36/ADC1_3)-->JOY4

D13-->JOYBUTTON1

D12-->JOYBUTTON2
