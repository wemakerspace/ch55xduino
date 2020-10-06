/*
  CMSIS_DAP

  Modified form ljbfly's implementation, working but a lot to improve
  RST   -> P30
  SWCLK -> P31
  SWDIO -> P32
  These pins works in OC mode with pull-up resistor (70K). Should be safe to connect 3.3V part.

  created 2020
  by Deqing Sun for use with CH55xduino

  This example code is in the public domain.

*/


#ifndef USER_USB_RAM
#error "This example needs to be compiled with a USER USB setting"
#endif

#include "src/CMSIS_DAPusb/DAP.h"

#define LED_BUILTIN 33

extern volatile __xdata uint8_t USBByteCountEP1;

extern __xdata uint8_t Ep0Buffer[];
extern __xdata uint8_t Ep1Buffer[];

void setup() {
  USBInit();
    pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (USBByteCountEP1) {
    USBByteCountEP1 = 0 ;

    bool sendHID = false;
digitalWrite(LED_BUILTIN, HIGH);
    if (Ep1Buffer[0] == 2) {
      memcpy(&Ep1Buffer[64], "1ALS", 4);
      UEP1_T_LEN = 4;
      UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //enable send
    }

    if (Ep1Buffer[0] == 4) {
      memcpy(&Ep1Buffer[64], "\1TEST\0\0", 7);
      UEP1_T_LEN = 7;
      UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //enable send
    }


    UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK; //enable receive

digitalWrite(LED_BUILTIN, LOW);

  }
}
