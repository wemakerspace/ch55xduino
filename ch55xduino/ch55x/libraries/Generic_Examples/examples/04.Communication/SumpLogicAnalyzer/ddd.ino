void sendP11CharDebug(char c) //8Mbps under 24M clk
{
    //uint8_t interruptOn = EA;
    //EA = 0;
    __asm__("  mov c,_EA         \n"
            "  clr a             \n"
            "  rlc a             \n"
            "  mov b,a           \n"
            "  clr _EA           \n");

    //using P1.4
    __asm__(  //any branch will cause unpredictable timing due to code alignment
            "  mov a,dpl         \n"  //the parameter of func
            
            "  clr c             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  rrc a             \n"
            "  mov _P1_1,c       \n"
            "  setb c            \n"
            "  mov _P1_1,c       \n"
            );
    //if (interruptOn) EA = 1;
    
    __asm__("  mov a,b           \n"
            "  jz skipSetEADebug$\n"
            "  setb _EA          \n"
            "skipSetEADebug$:    \n");
}
