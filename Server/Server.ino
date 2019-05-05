#include <VirtualWire.h>

int RxPin = 12;
int TxPin = 11;

void setup() {
  Serial.begin(9600);
  
  vw_set_ptt_inverted(true);
  //vw_set_rx_pin(RxPin);
  vw_set_tx_pin(TxPin);

  vw_setup(2000);
}

void loop() {
    
  char msg[7] = {'S','e','r','v','e','r','#'};
  
  if(vw_send((uint8_t *)msg, 7))
  {
    vw_wait_tx();
    Serial.println("Message Sent!");
  }
  delay(2000);
}
