//https://github.com/gillham/logic_analyzer/blob/master/logic_analyzer.ino
//https://www.sump.org/projects/analyzer/protocol/
//http://dangerousprototypes.com/docs/The_Logic_Sniffer%27s_extended_SUMP_protocol

#define LED_BUILTIN 33

/* XON/XOFF are not supported. */
#define SUMP_RESET 0x00
#define SUMP_ARM   0x01
#define SUMP_QUERY 0x02
#define SUMP_XON   0x11
#define SUMP_XOFF  0x13

/* mask & values used, config ignored. only stage0 supported */
#define SUMP_TRIGGER_MASK 0xC0
#define SUMP_TRIGGER_VALUES 0xC1
#define SUMP_TRIGGER_CONFIG 0xC2

/* Most flags (except RLE) are ignored. */
#define SUMP_SET_DIVIDER 0x80
#define SUMP_SET_READ_DELAY_COUNT 0x81
#define SUMP_SET_FLAGS 0x82
#define SUMP_SET_RLE 0x0100

/* extended commands -- self-test unsupported, but metadata is returned. */
#define SUMP_SELF_TEST 0x03
#define SUMP_GET_METADATA 0x04

#pragma callee_saves sendCharDebug
void sendCharDebug(char c);

uint8_t cmdByte = 0;
uint8_t cmdBytes[5];

void setup() {


  pinMode(LED_BUILTIN, OUTPUT);
}


void loop()
{
  int i;

  if (USBSerial_available() > 0) {
    cmdByte = USBSerial_read();

    sendCharDebug(cmdByte);//!!!!!
    switch (cmdByte) {
      case SUMP_RESET:  //0x00
        //   We don't do anything here as some unsupported extended commands have
        //   zero bytes and are mistaken as resets.  This can trigger false resets
        //   so we don't erase the data or do anything for a reset.
        break;
      case SUMP_QUERY:  //0x02
        // return the expected bytes.
        USBSerial_write('1');
        USBSerial_write('A');
        USBSerial_write('L');
        USBSerial_write('S');
        break;
      case SUMP_GET_METADATA:
        // We return a description of our capabilities.
        // Check the function's comments below.
        get_metadata();
        break;
      case SUMP_SELF_TEST:
        /* ignored. */
        break;
      default:
        // ignore any unrecognized bytes.
        break;
    }
  }
}

void blinkled() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

/*
   This function returns the metadata about our capabilities.  It is sent in
   response to the  OpenBench Logic Sniffer extended get metadata command.
*/
void get_metadata() {
  // device name
  USBSerial_write((uint8_t)0x01);
  USBSerial_write('A');
  USBSerial_write('G');
  USBSerial_write('L');
  USBSerial_write('A');
  USBSerial_write(' ');
  USBSerial_write('C');
  USBSerial_write('H');
  USBSerial_write('5');
  USBSerial_write('5');
  USBSerial_write('x');
  USBSerial_write(' ');
  USBSerial_write('v');
  USBSerial_write('0');
  USBSerial_write((uint8_t)0x00);

  // firmware version
  USBSerial_write((uint8_t)0x02);
  USBSerial_write('0');
  USBSerial_write('.');
  USBSerial_write('1');
  USBSerial_write('3');
  USBSerial_write((uint8_t)0x00);

  // sample memory
  USBSerial_write((uint8_t)0x21);
  USBSerial_write((uint8_t)0x00);
  USBSerial_write((uint8_t)0x00);
  // 532 bytes to change !!!!!?
  USBSerial_write((uint8_t)0x02);
  USBSerial_write((uint8_t)0x14);

  // sample rate (4MHz) !!!!!?
  USBSerial_write((uint8_t)0x23);
  USBSerial_write((uint8_t)0x00);
  USBSerial_write((uint8_t)0x3D);
  USBSerial_write((uint8_t)0x09);
  USBSerial_write((uint8_t)0x00);

  // number of probes (4 for ch55x)
  USBSerial_write((uint8_t)0x40);
  USBSerial_write((uint8_t)0x04);

  // protocol version (2)
  USBSerial_write((uint8_t)0x41);
  USBSerial_write((uint8_t)0x02);

  // end of data
  USBSerial_write((uint8_t)0x00);
}
