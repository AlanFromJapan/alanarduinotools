/*
 * vusb_config.h
 *
 * Created: 2014/12/20 17:01:26
 *  Author: Alan
 */ 


#ifndef VUSBCONFIG_H_
#define VUSBCONFIG_H_


#include <string.h>         // for memcpy()

#include "usbdrv.h"
#include "Keyboard.h"

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

//Need this empty signature, too lazy to check why
void usbEventResetReady(void){}


//
// msgbuf[] is 8 bytes long
//  byte0 = command
//  byte1..byte7 = args for command
//
void handleMessage(void)
{
	uint8_t* msgbufp = msgbuf+1;  // skip over report id

	//Get the message first byte and assign its value to PORTB.
	//Since the 3 highest bits(pins) are linked to leds, whatever char you send can lit the leds.
	PORTB = 	msgbufp[0];
}


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
 * Real job is done in handleMessage() above.
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

//Redirect messages to/from the host and manage callback of functions for USB
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




#endif /* VUSBCONFIG_H_ */