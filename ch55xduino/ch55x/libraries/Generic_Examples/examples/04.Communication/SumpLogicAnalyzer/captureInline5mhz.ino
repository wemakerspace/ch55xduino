
void captureInline5mhz() {


  /*
     basic trigger, wait until all trigger conditions are met on port.
     this needs further testing, but basic tests work as expected.
  */
  if (trigger) {
    while ((trigger_values ^ P1) & trigger);
  }

  /*
     disable interrupts during capture to maintain precision.
     we cannot afford any timing interference so we absolutely
     cannot have any interrupts firing.
  */
  EA = 0;

  /*
     toggle pin a few times to activate trigger for debugging.
     this is used during development to measure the sample intervals.
     it is best to just leave the toggling in place so we don't alter
     any timing unexpectedly.
  */
  /* DEBUG_ENABLE;
    DEBUG_ON;
    delayMicroseconds(1);
    DEBUG_OFF;
    delayMicroseconds(1);
    DEBUG_ON;
    delayMicroseconds(1);
    DEBUG_OFF;
    delayMicroseconds(1);

    DEBUG_ON; *//* debug timing measurement */

  /*
     Unroll loop to maximize capture speed.
     Pad with 1 NOP (1 cycle) to make this run at 4MHz.


  */

#undef INLINE_NOP
#define INLINE_NOP    __asm__("nop\n\t");


  //P1 is at address 0x90, but it can not be indirectly accessed, because it is on "top" of the ram

  //prepare R0 R7 DPTR
  __asm__("  mov r7,#0xF0      \n"
          "  mov r0,#_swapByte       \n"
          "  inc _XBUS_AUX       \n"  //select DPTR1
          "  mov dptr,#(_logicdata-0)       \n"
          "  dec _XBUS_AUX       \n"  //select DPTR0 , set it back
         );

  //a5-> MOVX @DPTR1,A & INC DPTR1

  __asm__("  mov a,_P1 \n  swap a \n mov @r0,a \n " );
  __asm__("  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n" );

  __asm__("  mov a,_P1 \n  swap a \n mov @r0,a \n " );
  __asm__("  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n" );


  __asm__("  mov a,_P1 \n  swap a \n mov @r0,a \n " );
  __asm__("  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n" );

  // __asm__("  mov a,@r0 \n  anl a,r7 \n  swap a \n  mov r6,a \n  nop \n  nop \n" );
  // __asm__("  mov a,@r0 \n  anl a,r7 \n  orl a,r6 \n  movx @dptr,a \n  inc dptr \n  nop \n" );
  //
  /*



    logicdata[0] = CHANPIN;
    INLINE_NOP;
    logicdata[1] = CHANPIN;
    INLINE_NOP;
    logicdata[2] = CHANPIN;
    INLINE_NOP;
    logicdata[3] = CHANPIN;
    INLINE_NOP;
    logicdata[4] = CHANPIN;
    INLINE_NOP;
    logicdata[5] = CHANPIN;
    INLINE_NOP;
    logicdata[6] = CHANPIN;
    INLINE_NOP;
    logicdata[7] = CHANPIN;
    INLINE_NOP;
    logicdata[8] = CHANPIN;
    INLINE_NOP;
    logicdata[9] = CHANPIN;
    INLINE_NOP;
    logicdata[10] = CHANPIN;
    INLINE_NOP;
    logicdata[11] = CHANPIN;
    INLINE_NOP;
    logicdata[12] = CHANPIN;
    INLINE_NOP;
    logicdata[13] = CHANPIN;
    INLINE_NOP;
    logicdata[14] = CHANPIN;
    INLINE_NOP;
    logicdata[15] = CHANPIN;
    INLINE_NOP;
    logicdata[16] = CHANPIN;
    INLINE_NOP;
    logicdata[17] = CHANPIN;
    INLINE_NOP;
    logicdata[18] = CHANPIN;
    INLINE_NOP;
    logicdata[19] = CHANPIN;
    INLINE_NOP;
    logicdata[20] = CHANPIN;
    INLINE_NOP;
    logicdata[21] = CHANPIN;
    INLINE_NOP;
    logicdata[22] = CHANPIN;
    INLINE_NOP;
    logicdata[23] = CHANPIN;*/


  /* re-enable interrupts now that we're done sampling. */
  EA = 1;

  /*
     dump the samples back to the SUMP client.  nothing special
     is done for any triggers, this is effectively the 0/100 buffer split.
  */
  /* for (i = 0 ; i < readCount; i++) {
     if ((i & 1) == 0) {
       USBSerial_write(logicdata[i/2] & 0x0F);
       //USBSerial_write(1);
     } else {
       USBSerial_write((logicdata[i/2]) >> 4);
       //USBSerial_write(0);
     }
    }*/

  //pulseview use reversed order
  for (int i = readCount ; i >= 0; i-- ) {
    if ((i & 1) == 0) {
      USBSerial_write(logicdata[i / 2] & 0x0F);
      //USBSerial_write(1);
    } else {
      USBSerial_write((logicdata[i / 2]) >> 4);
      //USBSerial_write(0);
    }
  }

}
