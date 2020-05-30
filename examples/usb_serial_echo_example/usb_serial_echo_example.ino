
__xdata char recvStr[64];
uint8_t recvStrPtr = 0;
bool stringComplete = false;
uint16_t echoCounter = 0;

void setup() {
  // put your setup code here, to run once:
}

void loop() {

  while (USBSerial_available()) {
    char serialChar = USBSerial_read();
    if ((serialChar == '\n') || (serialChar == '\r') ) {
      recvStr[recvStrPtr] = '\0';
      if (recvStrPtr > 0) {
        stringComplete = true;
        break;
      }
    } else {
      recvStr[recvStrPtr] = serialChar;
      recvStrPtr++;
      if (recvStrPtr == 63) {
        recvStr[recvStrPtr] = '\0';
        stringComplete = true;
        break;
      }
    }
  }

  if (stringComplete) {
    USBSerial_print_n("ECHO:", 5);
    USBSerial_print_n(recvStr, recvStrPtr);
    USBSerial_print_n("\n", 1);
    USBSerial_flush();
    stringComplete = false;
    recvStrPtr = 0;

    echoCounter++;
    //use sprintf to print formatted string, at cost ~2K flash
    __xdata char buf[20];
    int8_t len = sprintf(buf, "echo count: %d\n", echoCounter);
    if (len > 0) {
      USBSerial_print_n(buf, len);
      USBSerial_flush();
    }
  }
}
