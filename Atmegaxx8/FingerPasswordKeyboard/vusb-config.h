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


// HID descriptor, 1 report, 8 bytes long
const PROGMEM char usbHidReportDescriptor[63] = {	

	    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	    0x09, 0x06,                    // USAGE (Keyboard)
	    0xa1, 0x01,                    // COLLECTION (Application)
	    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
	    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
	    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
	    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	    0x75, 0x01,                    //   REPORT_SIZE (1)
	    0x95, 0x08,                    //   REPORT_COUNT (8)
	    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
	    0x95, 0x01,                    //   REPORT_COUNT (1)
	    0x75, 0x08,                    //   REPORT_SIZE (8)
	    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
	    0x95, 0x05,                    //   REPORT_COUNT (5)
	    0x75, 0x01,                    //   REPORT_SIZE (1)
	    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
	    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
	    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
	    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
	    0x95, 0x01,                    //   REPORT_COUNT (1)
	    0x75, 0x03,                    //   REPORT_SIZE (3)
	    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
	    0x95, 0x06,                    //   REPORT_COUNT (6)
	    0x75, 0x08,                    //   REPORT_SIZE (8)
	    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
	    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
	    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
	    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
	    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
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

	
	
}



static uchar    idleRate;           /* in 4 ms units */

//Redirect messages to/from the host and manage callback of functions for USB
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;
	usbMsgPtr = &keyboard_report;
    // HID class request 
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
        // wValue: ReportType (highbyte), ReportID (lowbyte)
        //uint8_t rid = rq->wValue.bytes[0];  // report Id
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  
            // since we have only one report type, we can ignore the report-ID

            return sizeof(keyboard_report);
        } else if(rq->bRequest == USBRQ_HID_SET_REPORT) {
            // since we have only one report type, we can ignore the report-ID 

            return USB_NO_MSG; // use usbFunctionWrite() to recv data from host 
        }
	    else if(rq->bRequest == USBRQ_HID_GET_IDLE){
		    usbMsgPtr = &idleRate;
		    return 1;
	    }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
		    idleRate = rq->wValue.bytes[1];
	    }
    }else{
        // ignore vendor type requests, we don't use any 
    }
    return 0;
}


// ------------------------------------------------------------------------- 




#endif /* VUSBCONFIG_H_ */