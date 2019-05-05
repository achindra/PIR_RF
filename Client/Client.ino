#include <VirtualWire.h>

int RxPin = 12;
int TxPin = 11;

void setup() {
  Serial.begin(9600);

  vw_set_ptt_inverted(true);
  vw_set_rx_pin(RxPin);
  //vw_set_tx_pin(TxPin);

  vw_setup(2000);
  vw_rx_start();
}

void loop() {

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  Serial.println("<Wait>");
  if (vw_wait_rx_max(200))
    Serial.println("</Wait>");

  if (vw_have_message())
  {
    Serial.println("HaveMsg: ");
    if (vw_get_message(buf, &buflen))
    {
      Serial.print("Rx: ");
      for (int i = 0; i < buflen; i++)
      {
        Serial.print(buf[i], HEX);
        Serial.print(' ');
      }
      Serial.println();
    }
  }

  if (vw_get_message(buf, &buflen))
  {
    Serial.print("Rx: ");
    for (int i = 0; i < buflen; i++)
    {
      Serial.print(buf[i], HEX);
      Serial.print(' ');
    }
    Serial.println();
  }
}
