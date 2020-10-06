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

//#include "src/Sump_HIDusb/USBHID.h"


extern volatile __xdata uint8_t USBByteCountEP1;
extern __xdata uint8_t Ep0Buffer[];
extern __xdata uint8_t Ep1Buffer[];

void setup() {
  pinMode(14, OUTPUT);
  USBInit();
  pinMode(33, OUTPUT);
  P3_3 = 0;
}

void loop() {

  /*if (digitalRead(34) == 0) {
    UEP1_T_LEN = 8;
    UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //enable send
    }*/

  if (USBByteCountEP1) {
    P3_3 = 1;

    if (Ep1Buffer[0] == 2) {
      memset(&Ep1Buffer[64], 0, 8);
      memcpy(&Ep1Buffer[64], "1ALS\9", 5);
      USBByteCountEP1 = 0 ;
      UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK; //enable receive
      delay(2);
      UEP1_T_LEN = 8;
      UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //enable send
    } else if (Ep1Buffer[0] == 4) {
      memset(&Ep1Buffer[64], 0, 8);
      memcpy(&Ep1Buffer[64], "\1TEST\0\0", 7);
      USBByteCountEP1 = 0 ;
      delay(10);
      UEP1_T_LEN = 8;
      UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //enable send
    } else {
      USBByteCountEP1 = 0 ;
      UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK; //enable receive
    }



    //DAP_Thread();



    P3_3 = 0;

    /*Ep1Buffer[64] = 0;
      UEP1_T_LEN = 8;
      UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //enable send*/
  }
}
