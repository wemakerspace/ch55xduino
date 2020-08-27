void captureInline5mhz() {

  //  basic trigger, wait until all trigger conditions are met on port.
  //  this needs further testing, but basic tests work as expected.

  //  disable interrupts during capture to maintain precision.
  //  we cannot afford any timing interference so we absolutely
  //  cannot have any interrupts firing.

  EA = 0;

  //prepare R0 DPTR
  __asm__("  mov r0,#_swapByte       \n"
          "  inc _XBUS_AUX       \n"  //select DPTR1
          "  mov dptr,#(_logicdata-0)       \n"
          "  dec _XBUS_AUX       \n"  //select DPTR0 , set it back
         );

  if (trigger) {
    while ((trigger_values ^ P1) & trigger);
  }

  //P1 is at address 0x90, but it can not be indirectly accessed, because it is on "top" of the ram

  //a5-> MOVX @DPTR1,A & INC DPTR1

  //===768 pairs
  //P3_3 = 1;

  __asm__("  mov r6,#153      \n"

          ".odd \n loop10Samples$:    \n" //even address is faster, not sure why need odd.

          "  mov a,_P1 \n  swap a \n mov @r0,a \n "
          "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n"

          "  mov a,_P1 \n  swap a \n mov @r0,a \n "
          "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n"

          "  nop      \n"

          "  mov a,_P1 \n  swap a \n mov @r0,a \n "
          "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n"

          "  nop      \n"

          "  mov a,_P1 \n  swap a \n mov @r0,a \n "
          "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n"

          "  dec r6      \n"

          "  mov a,_P1 \n  swap a \n mov @r0,a \n "
          "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n"

          "  mov a,r6      \n"  //1+2+2 = 5clk for loop
          "  jnz loop10Samples$\n");

  //P3_3 = 0;

  //765 pair finished, 3 more needed.
  __asm__(
    "  mov a,_P1 \n  swap a \n mov @r0,a \n "
    "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n"

    "  nop      \n"

    "  mov a,_P1 \n  swap a \n mov @r0,a \n "
    "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n"

    "  nop      \n"

    "  mov a,_P1 \n  swap a \n mov @r0,a \n "
    "  mov a,_P1 \n  xchd A,@r0 \n  .db #0xa5 \n");

  //===768 pairs end


  // re-enable interrupts now that we're done sampling.
  EA = 1;

  //   dump the samples back to the SUMP client.  nothing special
  //   is done for any triggers, this is effectively the 0/100 buffer split.

  //pulseview use reversed order

  for (unsigned int i = 0 ; i < readCount; i++ ) {
    unsigned int rev_i = readCount - i - 1;
    if ((rev_i & 1) == 0) {
      USBSerial_write(logicdata[rev_i / 2] & 0x0F);
    } else {
      USBSerial_write((logicdata[rev_i / 2]) >> 4);
    }
  }

}