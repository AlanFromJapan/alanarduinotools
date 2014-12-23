/*
 * vusb_config.h
 *
 * Created: 2014/12/20 17:01:26
 *  Author: Alan
 */ 


#ifndef VUSBCONFIG_H_
#define VUSBCONFIG_H_


#include "usbdrv.h"
#include "Keyboard.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

static uchar    reportBuffer[2];    /* buffer for HID reports */
static uchar    idleRate;           /* in 4 ms units */


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

/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and we only allow one
 * simultaneous key press (except modifiers). We can therefore use short
 * 2 byte input reports.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */


/* The following function returns an index for the first key pressed. It
* returns 0 if no key is pressed.
*/
uint8_t    keyPressed(void) {
	//input is pulled up, just one button so really don't care where it's pushed.
	//take the PINB input register, mask to keep the input pins and invert: should be 0x00 if unpressed
	return ~(0xe0 | (PINB & 0x1f));
}


static uchar protocolVer=1;/* 0 is the boot protocol, 1 is report protocol */

uint8_t	usbFunctionSetup(uint8_t data[8]){
	usbRequest_t    *rq = (void *)data;

	usbMsgPtr = reportBuffer;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
		if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
			/* we only have one report type, so don't look at wValue */
			buildReportEmpty();
			return sizeof(reportBuffer);
		}
		else 
		if(rq->bRequest == USBRQ_HID_GET_IDLE){
			usbMsgPtr = &idleRate;
			return 1;
		}
		else 
		if(rq->bRequest == USBRQ_HID_SET_IDLE){
			idleRate = rq->wValue.bytes[1];
		}
		
		//[Receive USB: you need this!]
		else if(rq->bRequest == USBRQ_HID_SET_REPORT){
			if (rq->wLength.word == 1) { // We expect one byte reports 
				return 0xFF; // Call usbFunctionWrite with data 
			}
		}
		
		//[Receive USB: you need this!]
		else if(rq->bRequest == USBRQ_HID_GET_PROTOCOL) {
		    if (rq->wValue.bytes[1] < 1) {
			    protocolVer = rq->wValue.bytes[1];
		    }
		}	   
		else if(rq->bRequest == USBRQ_HID_SET_PROTOCOL) {
		    usbMsgPtr = &protocolVer;
		    return 1;
	   }
		
	}
	else{
		/* no vendor specific requests implemented */
	}
	return 0;
}

//[Receive USB: you need this!]
//This method is in charge of handling the SET REPORT coming from the host
uint8_t usbFunctionWrite(uint8_t *data, uint8_t len) {
	
	//Toggle leds according status
	uint8_t vLedStatus = 0x00;
	if ((data[0] & LED_CAPS) != 0x00){
		vLedStatus |= 0x80;
	}
	if ((data[0] & LED_SCROLL) != 0x00){
		vLedStatus |= 0x40;
	}
	if ((data[0] & LED_NUM) != 0x00){
		vLedStatus |= 0x20;
	}
	
	//put the 3 high pins (the 3 leds) to 0 and "or" it with the ledmask to turn them on
	PORTB = (PORTB & 0x1f) | vLedStatus ;
	
	return 0x01;	
}

#endif /* VUSBCONFIG_H_ */