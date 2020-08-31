#include "USBconstant.h"


//Device descriptor
__code uint8_t DevDesc[] = {
    0x12,0x01,
    0x10,0x01,  //USB spec release number in BCD format, USB1.1 (0x10, 0x01).
    0x00,0x00,0x00, //bDeviceClass, bDeviceSubClass, bDeviceProtocol
    DEFAULT_ENDP0_SIZE, //PACKET_SIZE
    0x20,0x08,0x01,0x00, // VID PID
    0x00,0x01,  //version
    0x01,0x02,0x03, //bString
    0x01    //bNumConfigurations
};

__code uint16_t DevDescLen = sizeof(DevDesc);

__code uint8_t CfgDesc[] ={
    0x09,0x02,sizeof(CfgDesc) & 0xff,sizeof(CfgDesc) >> 8,
    0x01,               /* bNumInterfaces */
    0x01,               /* bConfigurationValue */
    0x00,               /* iConfiguration */
    0x80,               /* bmAttributes */
    0x64,               /* MaxPower */
    
    
    /* I/F descr: HID */
    0x09,      /* bLength */
    0x04,     /* bDescriptorType */
    0x00,               /* bInterfaceNumber */
    0x00,               /* bAlternateSetting */
    0x02,               /* bNumEndpoints */
    0x03,               /* bInterfaceClass */
    0x00,               /* bInterfaceSubClass */
    0x00,               /* bInterfaceProtocol */
    0x00,               /* iInterface */
    
    /* HID Descriptor */
    0x09,            /* Size of this descriptor in UINT8s. */
    0x21,           /* HID descriptor type. */
    0x10, 0x01,         /* HID Class Spec. release number. */
    0x00,               /* H/W target country. */
    0x01,               /* Number of HID class descriptors to follow. */
    0x22,       /* Descriptor type. */
    sizeof(ReportDesc) & 0xff,sizeof(ReportDesc) >> 8,    /* Total length of report descriptor. */
    
    /* EP Descriptor: interrupt in. */
    0x07,                       /* bLength */
    0x05,                      /* bDescriptorType */
    0x01,         /* bEndpointAddress */
    0x03,                             /* bmAttributes */
    0x08, 0x00,    /* wMaxPacketSize */
    1,        /* bInterval */
    
    /* EP Descriptor: interrupt out. */
    0x07,                       /* bLength */
    0x05,                      /* bDescriptorType */
    0x81,       /* bEndpointAddress */
    0x03,                             /* bmAttributes */
    0x08, 0x00,    /* wMaxPacketSize */
    1         /* bInterval */
    
    
};


__code uint16_t ReportDescLen = sizeof(ReportDesc);

__code uint8_t ReportDesc[] ={
    0x06, 0xAB, 0xFF,//RAWHID_USAGE_PAGE
    0x0A, 0x00, 0x02, //RAWHID_USAGE
    0xA1, 0x01,                     /* Collection 0x01 */
    0x75, 0x08,                     /* report size = 8 bits */
    0x15, 0x00,                     /* logical minimum = 0 */
    0x26, 0xFF, 0x00,               /* logical maximum = 255  */
    0x95, 8,              /* report count */
    0x09, 0x01,                     /* usage */
    0x81, 0x02,                     /* Input (array) */
    0x95, 8,              /* report count */
    0x09, 0x02,                     /* usage */
    0x91, 0x02,                     /* Output (array) */
    0xC0                            /* end collection */
};

__code uint16_t CfgDescLen = sizeof(CfgDesc);

//String Descriptors
__code uint8_t LangDes[]={0x04,0x03,0x09,0x04};           //Language Descriptor
__code uint16_t LangDesLen = sizeof(LangDes);
__code uint8_t SerDes[]={                                 //Serial String Descriptor
    0x0C,0x03,
    'C',0x00,'H',0x00,'5',0x00,'5',0x00,'x',0x00
};
__code uint16_t SerDesLen = sizeof(SerDes);
__code uint8_t Prod_Des[]={                                //Produce String Descriptor
    0x1E,0x03,
    'C',0x00,'H',0x00,'5',0x00,'5',0x00,'x',0x00,' ',0x00,
    'S',0x00,'U',0x00,'M',0x00,'P',0x00,' ',0x00,'H',0x00
    ,'I',0x00,'D',0x00
};
__code uint16_t Prod_DesLen = sizeof(Prod_Des);

__code uint8_t Manuf_Des[]={
    0x0E,0x03,
    'D',0x00,'e',0x00,'q',0x00,'i',0x00,'n',0x00,'g',0x00,
};
__code uint16_t Manuf_DesLen = sizeof(Manuf_Des);
