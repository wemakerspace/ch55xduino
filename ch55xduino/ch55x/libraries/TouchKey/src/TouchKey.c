/*
 * Copyright (c) 2020 by Deqing Sun <ds@thinkcreate.us> (c version for CH552 port)
 * Touch key library for arduino CH552.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */


#include "TouchKey.h"

typedef void (*voidFuncPtr)(void);
extern __xdata voidFuncPtr touchKeyHandler;

volatile __xdata uint16_t touchRawValue[6];
volatile __xdata uint8_t touchISRCounter = 0;
__xdata uint8_t channelEnabled = 0;

__xdata uint8_t touchCounterProcessed = 0;
__xdata uint8_t processIndex = 0;

__xdata uint8_t touchCycleCounter = 0;

__xdata uint16_t touchBaseline[6];
__xdata uint8_t touchBaselineNoiseCount[6];
__xdata uint8_t prevDiffBaselinePos;

__xdata uint8_t touchMaxHalfDelta;
__xdata uint8_t touchNoiseHalfDelta;
__xdata uint8_t touchNoiseCountLimit;
__xdata uint8_t touchFilterDelayLimit;

void TouchKey_ISR_Handler(void){
    uint8_t index = TKEY_CTRL & 0x07;
    touchRawValue[index-1]=TKEY_DAT;
    
    index++;
    if (index>6) index = 1;
    
    TKEY_CTRL = bTKC_2MS | index;
    touchISRCounter++;

}

//TIN0(P1.0), TIN1(P1.1), TIN2(P1.4), TIN3(P1.5), TIN4(P1.6), TIN5(P1.7)
void TouchKey_begin(uint8_t channelToEnableBitMask){
    
    channelEnabled = channelToEnableBitMask;
    
    //set IO to input
    if (channelEnabled & (1<<0)){
        P1_DIR_PU &= ~(1<<0);
        P1_MOD_OC &= ~(1<<0);
    }
    if (channelEnabled & (1<<1)){
        P1_DIR_PU &= ~(1<<1);
        P1_MOD_OC &= ~(1<<1);
    }
    if (channelEnabled & (1<<2)){
        P1_DIR_PU &= ~(1<<4);
        P1_MOD_OC &= ~(1<<4);
    }
    if (channelEnabled & (1<<3)){
        P1_DIR_PU &= ~(1<<5);
        P1_MOD_OC &= ~(1<<5);
    }
    if (channelEnabled & (1<<4)){
        P1_DIR_PU &= ~(1<<6);
        P1_MOD_OC &= ~(1<<6);
    }
    if (channelEnabled & (1<<5)){
        P1_DIR_PU &= ~(1<<7);
        P1_MOD_OC &= ~(1<<7);
    }
    
    touchMaxHalfDelta = 50;
    touchNoiseHalfDelta = 2;
    touchNoiseCountLimit = 10;
    
    
    //touchMaxHalfDelta = 0;
    //touchNoiseCountLimit = 50;
    //touchNoiseHalfDelta = 10;
    
    touchKeyHandler = TouchKey_ISR_Handler;
    
    TKEY_CTRL = bTKC_2MS | 1;
    
    IE_TKEY = 1;
}

uint8_t TouchKey_Process(){ //call this function every 12ms or less.
    int8_t sampleToProcess = touchISRCounter-touchCounterProcessed;
    if (sampleToProcess > 0){
        for (uint8_t i=0;i<sampleToProcess;i++){
            uint8_t indexBitMask = (1<<processIndex);
            if (channelEnabled & indexBitMask){                  //Baseline System refered to Freescale MPR121 AN3891
                uint8_t interruptOn = EA;
                EA = 0;
                uint16_t rawData = touchRawValue[processIndex];
                if (interruptOn) EA = 1;
                
                int16_t diff = rawData - touchBaseline[processIndex];
                
                if ( (diff<touchMaxHalfDelta) && (diff>-touchMaxHalfDelta) ){ //case 1. Small incremental changes to the system represent long term slow (environmental) changes in the system. 
                    touchBaseline[processIndex] = rawData;
                }else{
                    uint8_t noiseCount;
                    if (diff>0){
                        if ( (prevDiffBaselinePos&indexBitMask) ){  //same side noise, case 2
                            noiseCount = touchBaselineNoiseCount[processIndex];
                            noiseCount++;
                            if (noiseCount>=touchNoiseCountLimit){
                                noiseCount = 0;
                                touchBaseline[processIndex] += touchNoiseHalfDelta;
                            }
                            touchBaselineNoiseCount[processIndex] = noiseCount;
                        }else{  //different side noise, case 3
                            prevDiffBaselinePos|=indexBitMask;
                            touchBaselineNoiseCount[processIndex] = 1;
                        }
                    }else{
                         if ( (prevDiffBaselinePos&indexBitMask)==0 ){  //same side noise, case 2
                            noiseCount = touchBaselineNoiseCount[processIndex];
                            noiseCount++;
                            if (noiseCount>=touchNoiseCountLimit){
                                noiseCount = 0;
                                touchBaseline[processIndex] -= touchNoiseHalfDelta;
                            }
                            touchBaselineNoiseCount[processIndex] = noiseCount;
                        }else{  //different side noise, case 3
                            prevDiffBaselinePos&=~indexBitMask;
                            touchBaselineNoiseCount[processIndex] = 1;
                        }
                    }
                    
                    
   
                }
                
                


                
            }
            
            touchCounterProcessed++;
            processIndex++;
            if (processIndex>=6){
                processIndex = 0;
                touchCycleCounter++;
            }
        }
    }
    return touchCycleCounter;
}


void TouchKey_end(void){
    touchKeyHandler = NULL;
    TKEY_CTRL = 0;
    IE_TKEY = 0;
}