#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"


void sendCharDebug(char c);





void digitalWrite(uint8_t pin, uint8_t val)
{
	uint8_t timer = digitalPinToTimer(pin);
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
    
    // If the pin that support PWM output, we need to turn it off
	// before doing a digital write.
	//!!!!!!!if (timer != NOT_ON_TIMER) turnOffPWM(timer);
    
    //C pointers cannot be used to access the 8051's SFRs (special function registers).
    
	uint8_t interruptOn = EA;
    EA = 0;
    
    switch(port){
        case P1PORT:
            if (val == LOW) {
                P1 &= ~bit;
            } else {
                P1 |= bit;
            }
            break;
        case P2PORT:
            if (val == LOW) {
                P2 &= ~bit;
            } else {
                P2 |= bit;
            }
            break;
        case P3PORT:
            if (val == LOW) {
                P3 &= ~bit;
            } else {
                P3 |= bit;
            }
            break;
        default:
    }
    
    if (interruptOn) EA = 1;
}