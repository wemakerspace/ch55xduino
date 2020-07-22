/*
  CMSIS_DAP



  created 2020
  by Deqing Sun for use with CH55xduino

  This example code is in the public domain.

*/


#ifndef USER_USB_RAM
#error "This example needs to be compiled with a USER USB setting"
#endif

#include "src/CMSIS_DAPusb/USBHID.h"


extern volatile __xdata uint8_t USBByteCountEP1;


void setup() {
  USBInit();
}

void loop() {
  if (USBByteCountEP1) {
    USBByteCountEP1 = 0 ;
    if (USBByteCountEP1 == 0) {
      UEP1_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_ACK;
    }
  }




}
