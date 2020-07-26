/*
 Print.c - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.

 The float functions are moved to a separate file Print-float.c. This way
 we can keep the linker from pulling in all the float functions even when no
 floats are used in the sketch.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
 Modified 23 November 2006 by David A. Mellis
 Modified 03 August 2015 by Chuck Todd
 Modified 30 December 2016 by Michael Mayer
 */


//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
#include <math.h>
#include "Arduino.h"

#include "Print.h"



void printNL(void);

// Public Methods //////////////////////////////////////////////////////////////

uint8_t Print_print_sn(writefunc_p writefunc, uint8_t * __xdata buffer, __xdata uint8_t size)
{
    uint8_t n = 0;
    while (size--) {
        if (writefunc(*buffer++)) n++;
        else break;
    }
    return n;
}

uint8_t Print_print_s(writefunc_p writefunc, char * __xdata str)
{
    uint8_t        n = 0;
    char c;
    
    if (!str) return 0;
    
    while ( c=*str++ ) {    // assignment intented
        if (writefunc(c)) n++;
        else break;
    }
    return n;
}


uint8_t Print_print_u(writefunc_p writefunc, __xdata unsigned long n)
{
    return printNumber(writefunc, n,10);
}

uint8_t Print_print_i(writefunc_p writefunc, __xdata long n)
{
    return printInt(writefunc, n,10);
}

// (not so) Private Methods /////////////////////////////////////////////////////////////

uint8_t Print_println(writefunc_p writefunc)
{
    uint8_t n;
    
    n  = writefunc(13);
    n += writefunc(10);
    return n;
}


__code uint8_t div10Byte2QuotientLut[10] = {(uint8_t)(256 * 0 / 10), (uint8_t)(256 * 1 / 10), (uint8_t)(256 * 2 / 10), (uint8_t)(256 * 3 / 10), (uint8_t)(256 * 4 / 10), (uint8_t)(256 * 5 / 10), (uint8_t)(256 * 6 / 10), (uint8_t)(256 * 7 / 10), (uint8_t)(256 * 8 / 10), (uint8_t)(256 * 9 / 10)};
__code uint8_t div10Byte2RemainderLut[10] = {(uint8_t)(256 * 0 % 10), (uint8_t)(256 * 1 % 10), (uint8_t)(256 * 2 % 10), (uint8_t)(256 * 3 % 10), (uint8_t)(256 * 4 % 10), (uint8_t)(256 * 5 % 10), (uint8_t)(256 * 6 % 10), (uint8_t)(256 * 7 % 10), (uint8_t)(256 * 8 % 10), (uint8_t)(256 * 9 % 10)};
__code uint16_t div10Byte3QuotientLut[10] = {(uint16_t)(65536L * 0 / 10), (uint16_t)(65536L * 1 / 10), (uint16_t)(65536L * 2 / 10), (uint16_t)(65536L * 3 / 10), (uint16_t)(65536L * 4 / 10), (uint16_t)(65536L * 5 / 10), (uint16_t)(65536L * 6 / 10), (uint16_t)(65536L * 7 / 10), (uint16_t)(65536L * 8 / 10), (uint16_t)(65536L * 9 / 10)};
__code uint8_t div10Byte3RemainderLut[10] = {(uint8_t)(65536L * 0 % 10), (uint8_t)(65536L * 1 % 10), (uint8_t)(65536L * 2 % 10), (uint8_t)(65536L * 3 % 10), (uint8_t)(65536L * 4 % 10), (uint8_t)(65536L * 5 % 10), (uint8_t)(65536L * 6 % 10), (uint8_t)(65536L * 7 % 10), (uint8_t)(65536L * 8 % 10), (uint8_t)(65536L * 9 % 10)};
__code uint32_t div10Byte4QuotientLut[10] = {(uint32_t)(16777216L * 0 / 10), (uint32_t)(16777216L * 1 / 10), (uint32_t)(16777216L * 2 / 10), (uint32_t)(16777216L * 3 / 10), (uint32_t)(16777216L * 4 / 10), (uint32_t)(16777216L * 5 / 10), (uint32_t)(16777216L * 6 / 10), (uint32_t)(16777216L * 7 / 10), (uint32_t)(16777216L * 8 / 10), (uint32_t)(16777216L * 9 / 10)};
__code uint8_t div10Byte4RemainderLut[10] = {(uint8_t)(16777216L * 0 % 10), (uint8_t)(16777216L * 1 % 10), (uint8_t)(16777216L * 2 % 10), (uint8_t)(16777216L * 3 % 10), (uint8_t)(16777216L * 4 % 10), (uint8_t)(16777216L * 5 % 10), (uint8_t)(16777216L * 6 % 10), (uint8_t)(16777216L * 7 % 10), (uint8_t)(16777216L * 8 % 10), (uint8_t)(16777216L * 9 % 10)};

uint32_t uint32DivBy10(uint32_t n, __idata uint8_t *reminder) {
    n; reminder; //remove warning
    //n->’dpl’,’dph’,’b’ & ’acc’
    __asm__ ( ";Back up a, b to R3, R2                               \n"
             "    mov r3,a                                          \n"
             "    mov r2,b                                          \n"
             
             "    mov a,dpl                                         \n"
             "    mov b,#10                                         \n"
             "    div ab                                            \n"
             
             ";R4~R7 store result                                   \n"
             ";R0 store mod                                         \n"
             
             "    mov r4,a                                          \n"
             "    mov r0,b                                          \n"
             
             "    mov a,dph                                         \n"
             "    mov b,#10                                         \n"
             "    div ab                                            \n"
             "    mov r5,a                                          \n"
             
             ";use lut to calculate reminder on 2nd byte correctly  \n"
             "    mov dptr,#_div10Byte2QuotientLut                  \n"
             "    mov a,b                                           \n"
             "    movc a,@a+dptr                                    \n"
             
             ";add quotient from reminder on 2nd byte               \n"
             "    add a,r4                                          \n"
             "    mov r4,a                                          \n"
             "    clr a                                             \n"
             "    addc a,r5                                         \n"
             "    mov r5,a                                          \n"
             
             ";add reminder                                         \n"
             "    mov dptr,#_div10Byte2RemainderLut                 \n"
             "    mov a,b                                           \n"
             "    movc a,@a+dptr                                    \n"
             "    add a,r0                                          \n"
             "    mov r0,a                                          \n"
             
             ";do byte 3                                            \n"
             "    mov a,r2                                          \n"
             "    mov b,#10                                         \n"
             "    div ab                                            \n"
             "    mov r6,a                                          \n"
             
             ";use lut to calculate reminder on 3rd byte correctly  \n"
             "    mov dptr,#_div10Byte3QuotientLut                  \n"
             "    mov a,b                                           \n"
             ";a is within 0~9, use RL to double                    \n"
             "    rl a                                              \n"
             "    movc a,@a+dptr                                    \n"
             
             ";add quotient from reminder on 3rd byte               \n"
             "    add a,r4                                          \n"
             "    mov r4,a                                          \n"
             "    mov a,b                                           \n"
             "    rl a                                              \n"
             "    inc a                                             \n"
             "    movc a,@a+dptr                                    \n"
             "    addc a,r5                                         \n"
             "    mov r5,a                                          \n"
             "    clr a                                             \n"
             "    addc a,r6                                         \n"
             "    mov r6,a                                          \n"
             
             ";add reminder                                         \n"
             "    mov dptr,#_div10Byte3RemainderLut                 \n"
             "    mov a,b                                           \n"
             "    movc a,@a+dptr                                    \n"
             "    add a,r0                                          \n"
             "    mov r0,a                                          \n"
             
             ";do byte 4                                            \n"
             "    mov a,r3                                          \n"
             "    mov b,#10                                         \n"
             "    div ab                                            \n"
             "    mov r7,a                                          \n"
             
             ";use lut to calculate reminder on 4th byte correctly  \n"
             "    mov dptr,#_div10Byte4QuotientLut                  \n"
             "    mov a,b                                           \n"
             ";a is within 0~9, use RL to double                    \n"
             "    rl a                                              \n"
             "    rl a                                              \n"
             "    movc a,@a+dptr                                    \n"
             
             ";add quotient from reminder on 3rd byte               \n"
             "    add a,r4                                          \n"
             "    mov r4,a                                          \n"
             "    mov a,b                                           \n"
             "    rl a                                              \n"
             "    rl a                                              \n"
             "    inc a                                             \n"
             "    movc a,@a+dptr                                    \n"
             "    addc a,r5                                         \n"
             "    mov r5,a                                          \n"
             "    mov a,b                                           \n"
             "    rl a                                              \n"
             "    rl a                                              \n"
             "    inc a                                             \n"
             "    inc a                                             \n"
             "    movc a,@a+dptr                                    \n"
             "    addc a,r6                                         \n"
             "    mov r6,a                                          \n"
             "    clr a                                             \n"
             "    addc a,r7                                         \n"
             "    mov r7,a                                          \n"
             
             ";add reminder                                         \n"
             "    mov dptr,#_div10Byte3RemainderLut                 \n"
             "    mov a,b                                           \n"
             "    movc a,@a+dptr                                    \n"
             "    add a,r0                                          \n"
             "    mov r0,a                                          \n"
             
             
             ";div sum of reminder  \n"
             "    mov a,r0                                          \n"
             "    mov b,#10                                         \n"
             "    div ab                                            \n"
             "    add a,r4                                          \n"
             "    mov r4,a                                          \n"
             "    clr a                                             \n"
             "    addc a,r5                                         \n"
             "    mov r5,a                                          \n"
             "    clr a                                             \n"
             "    addc a,r6                                         \n"
             "    mov r6,a                                          \n"
             "    clr a                                             \n"
             "    addc a,r7                                         \n"
             "    mov r7,a                                          \n"
             
             ";final reminder                                       \n"
             "    mov a,_uint32DivBy10_PARM_2                       \n"
             "    mov r1,a                                          \n"
             "    mov a,b                                           \n"
             "    mov @r1,a                                         \n"
             
             "    mov dpl,r4                                        \n"
             "    mov dph,r5                                        \n"
             "    mov b,r6                                          \n"
             "    mov a,r7                                          \n"
             );
}



uint8_t Print_print_ub(writefunc_p writefunc, __xdata unsigned long n, __xdata uint8_t base)
{
    __xdata char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    __xdata char *str = &buf[sizeof(buf) - 1];
    __idata char c;
    
    *str = '\0';
    
    // prevent crash if called with base == 1
    if (base < 2) base = 10;
    
    do {
        switch (base){
            case 2:
                c = *((uint8_t *)(&n)) % 2; //if we don't do pointer casting and use n, sdcc will do meanless movement on high 3 bytes.
                n /= 2;
                break;
            case 8:
                c = *((uint8_t *)(&n)) % 8;
                n /= 8;
                break;
            case 16:
                c = *((uint8_t *)(&n)) % 16;
                n /= 16;
                break;
            default:
                //base 10 and other
                n = uint32DivBy10(n, &c);
        }

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);
    
    return Print_print_s(writefunc, str);
}

uint8_t Print_print_ib(writefunc_p writefunc, __xdata long n, __xdata uint8_t base)
{
    if (base == 0) {
        return writefunc((unsigned char) n);
    } else if (base == 10) {
        if (n < 0) {
            int t = writefunc('-');
            n = -n;
            return printNumber(writefunc, n, 10) + t;
        }
        return printNumber(writefunc, n, 10);
    } else {
        return printNumber(writefunc, n, base);
    }
}

