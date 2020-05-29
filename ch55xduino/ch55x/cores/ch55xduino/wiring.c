#include "wiring_private.h"



void init()
{
    //set internal clock 
	SAFE_MOD = 0x55;
    SAFE_MOD = 0xAA;
    
#if F_CPU == 32000000
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x07;  // 32MHz
#elif F_CPU == 24000000
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x06;  // 24MHz
#elif F_CPU == 16000000
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x05;  // 16MHz
#elif F_CPU == 12000000
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x04;  // 12MHz
#elif F_CPU == 6000000
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x03;  // 6MHz
#elif F_CPU == 3000000
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x02;  // 3MHz
#elif F_CPU == 750000
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x01;  // 750KHz
#elif F_CPU == 187500
    CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x00;  // 187.5MHz
#else
#warning F_CPU invalid or not set
#endif
    
    SAFE_MOD = 0x00;
}

