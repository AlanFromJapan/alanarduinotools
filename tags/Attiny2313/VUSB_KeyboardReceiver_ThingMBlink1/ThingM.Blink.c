/* 
 * blink(1) - BlinkM-like device with USB interface
 *  2012, Tod E. Kurt, http://thingm.com/ , http://todbot.com/blog/
 * 
 * Based on much code from:
 *   Project: hid-custom-rq example by  Christian Starkjohann
 *   LinkM : http://linkm.thingm.com/
 *
 * 
 * Note: blink(1) contains no code from BlinkM.  The circuit is different,
 * the PWM is hardware, not software, and the fading and pattern engine work
 * differently. 
 *
 * 
 */

#define F_CPU 12000000

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  // for sei() 
#include <avr/eeprom.h>
#include <util/delay.h>     // for _delay_ms() 
#include <string.h>         // for memcpy()
#include <inttypes.h>
#include <ctype.h>          // for toupper()

#include "usbdrv.h"

static uint8_t usbHasBeenSetup;





// ------------------------------------------------------------------------- 
// ----------------------------- USB interface ----------------------------- 
// ------------------------------------------------------------------------- 

#define REPORT_COUNT 8

// The following variables store the status of the current data transfer 
static uchar    currentAddress;
static uchar    bytesRemaining;

static uint8_t msgbuf[REPORT_COUNT+1];
//static uint8_t msgbufout[8];

// HID descriptor, 1 report, 8 bytes long
const PROGMEM char usbHidReportDescriptor[24] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x95, REPORT_COUNT,            //   REPORT_COUNT (8)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};

// USB serial number is unique per device, stored in EEPROM
//int usbDescriptorStringSerialNumber[] = {
	//USB_STRING_DESCRIPTOR_HEADER( USB_CFG_SERIAL_NUMBER_LEN ),
	//USB_CFG_SERIAL_NUMBER
//};


void usbEventResetReady(void)
{
}

void handleMessage(void);

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    if(len > bytesRemaining)
        len = bytesRemaining;
    memcpy( data, msgbuf + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return len;
}

/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0) {
        handleMessage();
        return 1;            // end of transfer 
    }
    if(len > bytesRemaining) 
        len = bytesRemaining;
    memcpy( msgbuf+currentAddress, data, len );
    currentAddress += len;
    bytesRemaining -= len;

    if(bytesRemaining == 0) {  // FIXME: inelegant
        handleMessage();
        return 1;            // end of transfer 
    }
    return bytesRemaining == 0;  // return 1 if this was the last chunk 
}

//
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;
    // HID class request 
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
        // wValue: ReportType (highbyte), ReportID (lowbyte)
        //uint8_t rid = rq->wValue.bytes[0];  // report Id
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  
            // since we have only one report type, we can ignore the report-ID
            bytesRemaining = REPORT_COUNT;
            currentAddress = 0;
            return USB_NO_MSG; // use usbFunctionRead() to obtain data 
        } else if(rq->bRequest == USBRQ_HID_SET_REPORT) {
            // since we have only one report type, we can ignore the report-ID 
            bytesRemaining = REPORT_COUNT;
            currentAddress = 0;
            return USB_NO_MSG; // use usbFunctionWrite() to recv data from host 
        }
    }else{
        // ignore vendor type requests, we don't use any 
    }
    return 0;
}


// ------------------------------------------------------------------------- 


//
// msgbuf[] is 8 bytes long
//  byte0 = command
//  byte1..byte7 = args for command
//
// Available commands: ('x' == implemented)
// x Fade to RGB color       format: {'c', r,g,b,      th,tl, 0,0 }
// x Set RGB color now       format: {'n', r,g,b,        0,0, 0,0 }
// x Serverdown tickle/off   format: {'D', {1/0},th,tl,  0,0, 0,0 }
// x Play/Pause              format: {'p', {1/0},pos,0,  0,0, 0,0 }
// x Set pattern entry       format: {'P', r,g,b, th,tl, i,0 }
// - Read playback loc n
// - Set log2lin vals        format: {'M', i, v0, v1,v2,v3,v4, 0,0 } 
// - Get log2lin val 
// x Read EEPROM location    format: {'e' addr, }
// x Write EEPROM location   format: {'E', addr, val, }
// x Get version             format: {'v',} 
//
// // Save last N cmds for playback : 
//

//
void handleMessage(void)
{
    uint8_t* msgbufp = msgbuf+1;  // skip over report id

	//uint8_t vNonNullPos = 0x00; uint8_t vFound = 0x00;
	//for (uint8_t i = 0; i < REPORT_COUNT; i++){
		//if(msgbufp[i] != 0x00) {vNonNullPos = i; vFound = 1; break;}
	//}		
	//if (vFound) {
		//vNonNullPos = vNonNullPos << 5;
		//PORTB = vNonNullPos;
	//}
	
	
	//for (uint8_t i = 0; i < REPORT_COUNT; i++){
		//if(msgbufp[i] != 0x00) {PORTB = msgbufp[i]; break;}
	//}
	
	//confirm : the set red message gives 'n' in msgbufp[0]
	//for (uint8_t i = 0; i < REPORT_COUNT; i++){
		//if(msgbufp[i] != 0x00) {i += 1; PORTB = (i << 1); break;}
	//}
	
	PORTB = 	msgbufp[0];
}

// ------------------------------------------------------------------------- 
// ------------------------ chip setup -------------------------------------
// -------------------------------------------------------------------------

//
// timerInit -- initialize the various PWM & timekeeping functions
//  there are 3 PWMs to be setup and one timer for "millis" counting 
//
static void timerInit(void)
{
uint8_t	i, j;

PORTB = 0x1f;   /* activate all pull-ups EXCEPT on leds */
DDRB = 0xe0;       /* all pins input EXCEPT on leds that are outputs */

PORTD = 0xfa;   /* 1111 1010 bin: activate pull-ups except on USB lines */
DDRD = 0x07;    /* 0000 0111 bin: all pins input except USB (-> USB reset) */
j = 0;
while(--j){     /* USB Reset by device only required on Watchdog Reset */
i = 0;
while(--i); /* delay >10ms for USB reset */
	}
    DDRD = 0x02;    /* 0000 0010 bin: remove USB reset condition */
    /* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */
    TCCR0A = 5;      /* timer 0 prescaler: 1024 */
}


// ------------------------------------------------------------------------- 
// -------------------------- main logic -----------------------------------
// -------------------------------------------------------------------------


//
int main(void)
{
    wdt_enable(WDTO_1S);  // watchdog status is preserved on reset


    timerInit();
   
    usbInit();
    usbDeviceDisconnect();

    // fake USB disconnect for > 250 ms 
    for( uint8_t i=255; i>0; i-- ) {
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();

    sei();

    for(;;){                // main event loop 
        wdt_reset();
        usbPoll();
    }
    return 0;
}




// -eof-
