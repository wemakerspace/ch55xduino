

#ifndef Pins_Arduino_Include_h
#define Pins_Arduino_Include_h

#ifndef _BV
#define _BV(X) (1<<(X))
#endif

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_AN_INTERRUPT -1


#define P0PORT 1
#define P1PORT 2
#define P2PORT 3
#define P3PORT 4


enum {
    NOT_ON_PWM = 0,
    PIN_PWM1,
    PIN_PWM2,
    PIN_PWM1_,
    PIN_PWM2_,
    NUM_PWM_PINS
};


extern __code uint8_t digital_pin_to_timer_PGM[];

extern __code uint8_t PROGMEM digital_pin_to_port_PGM[];

extern __code uint8_t digital_pin_to_bit_mask_PGM[];

#endif
