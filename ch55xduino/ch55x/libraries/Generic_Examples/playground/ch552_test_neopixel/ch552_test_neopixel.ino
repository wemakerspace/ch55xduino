

#define LED_BUILTIN 33

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(15, OUTPUT);
}
__xdata uint8_t ledData[8];
void neopixel_show_long_P1_5(uint32_t dataAndLen) {
  //’dpl’ (LSB),’dph’,’b’ & ’acc’
  //DPTR is the array address, B is the low byte of length
#if F_CPU == 24000000
  __asm__ ("    mov r3, b                           \n"

           ";save EA to R6                          \n"
           "    mov c,_EA                           \n"
           "    clr a                               \n"
           "    rlc a                               \n"
           "    mov r6, a                           \n"
           ";disable interrupt                      \n"
           "    clr _EA                             \n"

           //even may skip a byte, may leaving it 0xFF, and the MOV R7,A may affect R7
           //CH552 can save 1 instruction of jump/branch insctruction go to an even addr
           ".even                                   \n"
           "startNewByte$:                          \n"
           "    movx  a,@dptr                       \n"
           "    inc dptr                            \n"
           "loopbit$:                               \n"
           "    setb _P1_5                          \n"
           "    rlc a                               \n"
           "    nop                                 \n"  //make it even
           "    jnc bit7skipLowNop$                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "bit7skipLowNop$:                        \n"
           "    clr _P1_5                           \n"
           "    jc bit7skipHighNop$                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "    nop                                 \n"
           "bit7skipHighNop$:                       \n"
           "    anl ar2,#7                          \n"

           "    djnz r2,loopbit$                    \n"
           "    djnz r3,startNewByte$               \n"
           "    nop                                 \n"
           "    setb _P1_5                          \n"

           ";restore EA from R6                     \n"
           "    mov a,r6                            \n"
           "    jz  skipRestoreEA_NP$               \n"
           "    setb  _EA                           \n"
           "skipRestoreEA_NP$:                      \n"

          );
#else
#error Only support 24M clock for neopixel now
#endif
}

// the loop function runs over and over again forever
void loop() {
  ledData[0] = 0x0F;  //G
  ledData[1] = 0x00;  //R
  ledData[2] = 0x0F;  //B

  neopixel_show_long_P1_5((((uint16_t)ledData) & 0xFFFF) | (((uint32_t)3 & 0xFF) << 16));
  delay(10);
}
