//https://github.com/gillham/logic_analyzer/blob/master/logic_analyzer.ino
//https://www.sump.org/projects/analyzer/protocol/
//http://dangerousprototypes.com/docs/The_Logic_Sniffer%27s_extended_SUMP_protocol

/*
   Function prototypes so this can compile from the cli.
   You'll need the 'arduino-core' package and to check the paths in the
   Makefile.
*/

//void triggerMicro(void);
//void captureMicro(void);
//void captureMilli(void);
void getCmd(void);
//void setupDelay(void);
void blinkled(void);
void get_metadata(void);
//void debugprint(void);
//void debugdump(void);
//void prettydump(void);
//void captureInline4mhz(void);
//void captureInline2mhz(void);

#define LED_BUILTIN 33

/* XON/XOFF are not supported. */
#define SUMP_RESET 0x00
#define SUMP_ARM   0x01
#define SUMP_QUERY 0x02
#define SUMP_RETURN_CAPTURE_DATA  0x08
#define SUMP_XON   0x11
#define SUMP_XOFF  0x13

/* mask & values used, config ignored. only stage0 supported */
#define SUMP_TRIGGER_MASK 0xC0
#define SUMP_TRIGGER_VALUES 0xC1
#define SUMP_TRIGGER_CONFIG 0xC2

#define SUMP_TRIGGER_MASK_2 0xC4
#define SUMP_TRIGGER_MASK_3 0xC8
#define SUMP_TRIGGER_MASK_4 0xCC
#define SUMP_TRIGGER_VALUES_2 0xC5
#define SUMP_TRIGGER_VALUES_3 0xC9
#define SUMP_TRIGGER_VALUES_4 0xCD
#define SUMP_TRIGGER_CONFIG_2 0xC6
#define SUMP_TRIGGER_CONFIG_3 0xCA
#define SUMP_TRIGGER_CONFIG_4 0xCE

/* Most flags (except RLE) are ignored. */
#define SUMP_SET_DIVIDER 0x80
#define SUMP_SET_READ_DELAY_COUNT 0x81
#define SUMP_SET_FLAGS 0x82
#define SUMP_SET_RLE 0x0100

/* extended commands -- self-test unsupported, but metadata is returned. */
#define SUMP_SELF_TEST 0x03
#define SUMP_GET_METADATA 0x04




//#define DEBUG_MENU
//#define DEBUG

#ifdef DEBUG
#define MAX_CAPTURE_SIZE (768-128)
#else
#define MAX_CAPTURE_SIZE (768*2)
#endif // DEBUG 


// SUMP command from host (via serial)
// SUMP commands are either 1 byte, or for the extended commands, 5 bytes.

uint8_t cmdByte = 0;
uint8_t cmdBytes[5];

#ifdef DEBUG
byte savebytes[128];
int savecount = 0;
#endif // DEBUG 

__xdata uint8_t logicdata[MAX_CAPTURE_SIZE / 2];
unsigned int logicIndex = 0;
unsigned int triggerIndex = 0;
unsigned int readCount = MAX_CAPTURE_SIZE;
unsigned int delayCount = 0;
uint8_t trigger = 0;
uint8_t trigger_values = 0;
unsigned int useMicro = 0;
unsigned int delayTime = 0;
unsigned long divider = 0;
boolean rleEnabled = 0;

__idata uint8_t swapByte; //for XCHD instruction

#pragma callee_saves sendP11CharDebug
void sendP11CharDebug(char c);





void setup() {


  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(11, OUTPUT);//!!!!!
}


void loop()
{
  int i;

  if (USBSerial_available() > 0) {
    cmdByte = USBSerial_read();
    sendP11CharDebug('C');
    sendP11CharDebug(cmdByte);//!!!!!
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
      case SUMP_ARM:    //0x01
        // Zero out any previous samples before arming.
        // Done here instead via reset due to spurious resets.

        for (i = 0 ; i < (sizeof(logicdata) / sizeof(logicdata[0])); i++) {
          logicdata[i] = 0;
        }
        //   depending on the sample rate we need to delay in microseconds
        //   or milliseconds.  We can't do the complex trigger at 1MHz
        //   so in that case (delayTime == 1 and triggers enabled) use
        //   captureMicro() instead of triggerMicro().
        if (true || divider == 24) {
          // 4.0MHz
          captureInline6mhz();
        }
        /*       else if (divider == 49) {
                 // 2.0MHz
          #if !defined(__AVR_ATmega168__)
                 captureInline2mhz();
          #endif
               }
               else if (useMicro) {
                 if (trigger && (delayTime != 1)) {
                   triggerMicro();
                 }
                 else {
                   captureMicro();
                 }
               }
               else {
                 captureMilli();
               }*/
        break;

      case SUMP_RETURN_CAPTURE_DATA:  //0x08
        break;

      case SUMP_TRIGGER_MASK:  //0xC0
        //  the trigger mask byte has a '1' for each enabled trigger so
        //   we can just use it directly as our trigger mask.
        getCmd();

        trigger = cmdBytes[0] << 4; //we use P14~P17

        sendP11CharDebug('T'); sendP11CharDebug(trigger);

        break;
      case SUMP_TRIGGER_VALUES:  //0xC1
        // trigger_values can be used directly as the value of each bit
        // defines whether we're looking for it to be high or low.
        getCmd();
        trigger_values = cmdBytes[0] << 4;

        sendP11CharDebug('V'); sendP11CharDebug(trigger_values);
        break;
      case SUMP_TRIGGER_CONFIG:  //0xC1
        // read the rest of the command bytes, but ignore them.
        getCmd();
        break;
      case SUMP_SET_DIVIDER: //0x80
        // the shifting needs to be done on the 32bit unsigned long variable
        // so that << 16 doesn't end up as zero.
        // f = clock / (x + 1), f seems to be 100M
        getCmd();
        divider = cmdBytes[2];
        divider = divider << 8;
        divider += cmdBytes[1];
        divider = divider << 8;
        divider += cmdBytes[0];
        sendP11CharDebug('D'); sendP11CharDebug(cmdBytes[0]); sendP11CharDebug(cmdBytes[1]); sendP11CharDebug(cmdBytes[2]);
        //!!!!!setupDelay();
        break;
      case SUMP_SET_READ_DELAY_COUNT:
        /*
             this just sets up how many samples there should be before
           and after the trigger fires.  The readCount is total samples
           to return and delayCount number of samples after the trigger.
           this sets the buffer splits like 0/100, 25/75, 50/50
           for example if readCount == delayCount then we should
           return all samples starting from the trigger point.
           if delayCount < readCount we return (readCount - delayCount) of
           samples from before the trigger fired.
        */
        getCmd();
        readCount = 4 * (((cmdBytes[1] << 8) | cmdBytes[0]) + 1);
        if (readCount > MAX_CAPTURE_SIZE)
          readCount = MAX_CAPTURE_SIZE;
        delayCount = 4 * (((cmdBytes[3] << 8) | cmdBytes[2]) + 1);
        if (delayCount > MAX_CAPTURE_SIZE)
          delayCount = MAX_CAPTURE_SIZE;
        sendP11CharDebug('n'); sendP11CharDebug(cmdBytes[0]); sendP11CharDebug(cmdBytes[1]); sendP11CharDebug(cmdBytes[2]); sendP11CharDebug(cmdBytes[3]);
        break;
      case SUMP_TRIGGER_MASK_2:
      case SUMP_TRIGGER_MASK_3:
      case SUMP_TRIGGER_MASK_4:
      case SUMP_TRIGGER_VALUES_2:
      case SUMP_TRIGGER_VALUES_3:
      case SUMP_TRIGGER_VALUES_4:
      case SUMP_TRIGGER_CONFIG_2:
      case SUMP_TRIGGER_CONFIG_3:
      case SUMP_TRIGGER_CONFIG_4:
        getCmd();
        //ignore
        break;

      case SUMP_SET_FLAGS:
        /* read the rest of the command bytes and check if RLE is enabled. */
        getCmd();
        rleEnabled = ((cmdBytes[1] & 0b1000000) != 0);
        sendP11CharDebug('F'); sendP11CharDebug(cmdBytes[0]); sendP11CharDebug(cmdBytes[1]);
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
   Extended SUMP commands are 5 bytes.  A command byte followed by 4 bytes
   of options. We already read the command byte, this gets the remaining
   4 bytes of the command.
   If we're debugging we save the received commands in a debug buffer.
   We need to make sure we don't overrun the debug buffer.
*/
void getCmd() {
  delay(10);  //!!!!!!todo: reduce
  cmdBytes[0] = USBSerial_read();
  cmdBytes[1] = USBSerial_read();
  cmdBytes[2] = USBSerial_read();
  cmdBytes[3] = USBSerial_read();

#ifdef DEBUG
  if (savecount < 120 ) {
    savebytes[savecount++] = ' ';
    savebytes[savecount++] = cmdByte;
    savebytes[savecount++] = cmdBytes[0];
    savebytes[savecount++] = cmdBytes[1];
    savebytes[savecount++] = cmdBytes[2];
    savebytes[savecount++] = cmdBytes[3];
  }
#endif /* DEBUG */
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
  // 1536 bytes
  USBSerial_write((uint8_t)0x06);
  USBSerial_write((uint8_t)0x00);

  // sample rate (5MHz)
  USBSerial_write((uint8_t)0x23);
  USBSerial_write((uint8_t)0x00);
  USBSerial_write((uint8_t)0x4C);
  USBSerial_write((uint8_t)0x4B);
  USBSerial_write((uint8_t)0x40);

  // number of probes (4 for ch55x)
  USBSerial_write((uint8_t)0x40);
  USBSerial_write((uint8_t)0x04);

  // protocol version (2)
  USBSerial_write((uint8_t)0x41);
  USBSerial_write((uint8_t)0x02);

  // end of data
  USBSerial_write((uint8_t)0x00);
}
