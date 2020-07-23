#include <stdint.h>
#include <stdbool.h>
#include "include/ch554.h"
#include "include/ch554_usb.h"
#include "USBconstant.h"
#include "USBhandler.h"

extern __xdata  uint8_t  Ep0Buffer[];
extern __xdata  uint8_t  Ep1Buffer[];

volatile __xdata uint8_t USBByteCountEP1 = 0;      //Bytes of received data on USB endpoint

void sendCharDebug(char c);//!!!!!!!!!!!

void USBInit(){
    USBDeviceCfg();                                                       //Device mode configuration
    USBDeviceEndPointCfg();                                               //Endpoint configuration   
    USBDeviceIntCfg();                                                    //Interrupt configuration    
    UEP0_T_LEN = 0;
    UEP1_T_LEN = 0;                                                       //Pre-use send length must be cleared	  
    UEP2_T_LEN = 0;                                                          
}


void USB_EP1_IN(){
    
    sendCharDebug('i');
    
    
    UEP1_T_LEN = 0;                                                    // No data to send anymore
    UEP1_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //Respond NAK by default

}

void USB_EP1_OUT(){
    sendCharDebug('o');
    
    if ( U_TOG_OK ){               // Discard unsynchronized packets
        
        USBByteCountEP1 = USB_RX_LEN;
        if (USBByteCountEP1){
            
            for (uint8_t i=0;i<USB_RX_LEN;i++){
                sendCharDebug(Ep1Buffer[i]);
            }
            
            UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_NAK;       //Respond NAK after a packet. Let main code change response after handling.
        }
    }
}

