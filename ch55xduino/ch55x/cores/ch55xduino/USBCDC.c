#include <stdint.h>
#include "include/ch554.h"
#include "include/ch554_usb.h"

extern __xdata __at (EP0_ADDR) uint8_t  Ep0Buffer[];

#define LINE_CODEING_SIZE 7
__xdata uint8_t LineCoding[LINE_CODEING_SIZE]={0x00,0xe1,0x00,0x00,0x00,0x00,0x08};   //初始化波特率为57600，1停止位，无校验，8数据位。

volatile __xdata uint8_t USBByteCountEP2 = 0;      //代表USB端点接收到的数据
volatile __xdata uint8_t USBBufOutPointEP2 = 0;    //取数据指针

volatile __xdata uint8_t UpPoint2_Busy  = 0;   //上传端点是否忙标志
volatile __xdata uint8_t controlLineState = 0;


void resetCDCParameters(){

    USBByteCountEP2 = 0;       //USB端点收到的长度
    UpPoint2_Busy = 0;
}

void setLineCodingHandler(){
    for (uint8_t i=0;i<((LINE_CODEING_SIZE<=USB_RX_LEN)?LINE_CODEING_SIZE:USB_RX_LEN);i++){
        LineCoding[i] = Ep0Buffer[i];
    }

    //!!!!!Config_Uart0(LineCoding);
}

uint16_t getLineCodingHandler(){
    uint16_t returnLen;

    returnLen = LINE_CODEING_SIZE;
    for (uint8_t i=0;i<returnLen;i++){
        Ep0Buffer[i] = LineCoding[i];
    }

    return returnLen;
}

void setControlLineStateHandler(){
    controlLineState = Ep0Buffer[2];
}


void USB_EP2_IN(){
    UEP2_T_LEN = 0;                                                    //预使用发送长度一定要清空
    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
    UpPoint2_Busy = 0;                                                  //清除忙标志
}

void USB_EP2_OUT(){
    if ( U_TOG_OK )                                                     // 不同步的数据包将丢弃
    {
        USBByteCountEP2 = USB_RX_LEN;
        USBBufOutPointEP2 = 0;                                             //取数据指针复位
        if (USBByteCountEP2)    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_R_RES | UEP_R_RES_NAK;       //收到一包数据就NAK，主函数处理完，由主函数修改响应方式
    }
}