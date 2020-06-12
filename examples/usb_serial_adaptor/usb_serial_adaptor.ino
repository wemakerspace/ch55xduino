extern __xdata uint8_t LineCoding[7]; //lineCoding of CDC is located in this array
__xdata uint8_t LineCodingOld[7];

void setup() {
  Serial0_begin(9600);
  memset(LineCodingOld, 0, 7);
}

void loop() {
  while (USBSerial_available()) {
    char serialChar = USBSerial_read();
    Serial0_write(serialChar);
  }
  while (Serial0_available()) {
    char serialChar = Serial0_read();
    USBSerial_write(serialChar);
  }

  if (memcmp ( LineCoding, LineCodingOld, 7 ) != 0) {
    memcpy(LineCodingOld, LineCoding, 7);
    uint32_t uart0_buad = 0;
    *((uint8_t *)&uart0_buad) = LineCoding[0];
    *((uint8_t *)&uart0_buad+1) = LineCoding[1];
    *((uint8_t *)&uart0_buad+2) = LineCoding[2];
    *((uint8_t *)&uart0_buad+3) = LineCoding[3];
    
    Serial0_begin(uart0_buad);
  }

}
