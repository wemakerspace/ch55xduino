volatile uint8_t int0Count = 0;
uint8_t int0CountOld = 0;
volatile uint8_t int1Count = 0;
uint8_t int1CountOld = 0;

void int0Callback() {
  int0Count++;
}
void int1Callback() {
  int1Count++;
}

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(0, int0Callback, FALLING);//P3.2
  attachInterrupt(1, int1Callback, FALLING);//P3.3
}

void loop() {
  //if you see a number increases a lot when you tie the pin Low with something mechanical, it is caused by bouncing. A small capacitor between pin and GND may help.
  if (int0Count != int0CountOld) {
    int0CountOld = int0Count;
    USBSerial_print_s("Int0 triggered: ");
    USBSerial_println_i(int0Count);
    
    if (int0Count>=5){
        USBSerial_println_s("Int0 detached");
        detachInterrupt(0);
    }
  }
  if (int1Count != int1CountOld) {
    int1CountOld = int1Count;
    USBSerial_print_s("Int1 triggered: ");
    USBSerial_println_i(int1Count);
  }

}
