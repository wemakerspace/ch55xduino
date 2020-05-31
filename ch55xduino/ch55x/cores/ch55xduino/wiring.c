#include "wiring_private.h"
#include "pins_arduino.h"   //only include once

void mDelaymS( uint16_t n );    //used for delay without timer

void USBDeviceCfg();
void USBDeviceIntCfg();
void USBDeviceEndPointCfg();

extern __idata volatile uint32_t timer0_millis;

uint32_t millis()
{
	unsigned long m;
	uint8_t interruptOn = EA;
    EA = 0;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	m = timer0_millis;
	if (interruptOn) EA = 1;
	return m;
}

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
    
    mDelaymS(5); //needed to stablize internal RC
    
	//init USB
    USBDeviceCfg();
    USBDeviceEndPointCfg();                                               //????
    USBDeviceIntCfg();                                                    //?????
    UEP0_T_LEN = 0;
    UEP1_T_LEN = 0;                                                       //????????????
    UEP2_T_LEN = 0;                                                       //????????????
	
	//init PWM
	PWM_CK_SE = 93;		//DIV by 94 for 1K freq on 24M clk
	PWM_CTRL = 0;
	
	//init T0 for millis
	TMOD = (TMOD & ~0x0F)|(bT0_M1);//mode 2 for autoreload 
	T2MOD = T2MOD & ~bT0_CLK;	//bT0_CLK=0;clk Div by 12
	TH0 = 255-250+1;
	TF0 = 0;
	ET0 = 1;
	TR0 = 1;
}

