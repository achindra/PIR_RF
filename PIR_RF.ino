#include <VirtualWire.h>

#define VW_MAX_MESSAGE_LEN 255

/*

1. PIR based motion detection
2. IR Control
3. Wireless communication

*/

int CalibrationTime = 30;
int PirPin = 4;
int PirReact = 5;
int PirStatus = 0;
unsigned long LastMotion = 0;
unsigned long Threshold = 5000;
//int RxPort = 0; // Interrupt Pin - 2 and 3
int TxPort = 8;
int RxStatus = 0;
int RxReact = 10;
int RxPin = 2;
int TxPin = 3;


void setup() {
  Serial.begin(9600);
  Serial.write("\nSensor Init\n");
  
  // PIR
  pinMode(PirPin, INPUT);
  pinMode(PirReact, OUTPUT);
  pinMode(RxReact, OUTPUT);
  
  //Calibrate Sensor
  digitalWrite(PirPin, LOW);
  digitalWrite(PirReact, LOW);
  digitalWrite(RxReact, LOW);
  
  for(int i = 0; i<CalibrationTime; i++)
  {
    PirStatus ^= 1;
    (PirStatus)? digitalWrite(PirReact, LOW) : digitalWrite(PirReact, HIGH);
    Serial.write(".");
    delay(1000);
  }
  Serial.write("\nSensor Online\n");
  digitalWrite(PirReact, LOW);
  
  vw_set_rx_pin(RxPin);
  vw_set_tx_pin(TxPin);
  vw_set_ptt_inverted(true);
  vw_setup(4000);
  
  vw_rx_start();
  
  //attachInterrupt(RxPort, RxIntReceive, RISING);
}

void loop() {
  // PIR
  if(digitalRead(PirPin) == HIGH)
  {
    //PIR Action
      LastMotion = millis();
      digitalWrite(PirReact, HIGH);
      delay(50);
      Serial.write(".");
  }
  else
  {
    if((millis() - LastMotion) > Threshold)
    {
      //PIR Action
        digitalWrite(PirReact, LOW);
        delay(50);
        Serial.write("\n");
    }
  }
  
  char msg[7] = {'h','e','l','l','o',' ','#'};
  vw_send((uint8_t *)msg, 7);
  vw_wait_tx();
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if(vw_have_message())
  {
    if (vw_get_message(buf, &buflen))
    {
      for (int i = 0; i < buflen; i++)
      {
        Serial.print(buf[i], HEX);
        Serial.print(' ');
      }
      Serial.println();
    }
  }
}

void RxIntReceive()
{
  //
  RxStatus ^= 1;
  (RxStatus)?   digitalWrite(RxReact, LOW) :   digitalWrite(RxReact, HIGH);
}

