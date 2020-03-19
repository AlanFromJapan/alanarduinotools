/*
 * vusb_config.h
 *
 * Created: 2014/12/20 17:01:26
 *  Author: Alan
 */ 


#ifndef VUSBCONFIG_H_
#define VUSBCONFIG_H_

#define DBG1(a,b,c) ;

#include <string.h>         // for memcpy()

#include "usbdrv.h"

// -------------------------------------------------------------------------
// ----------------------------- USB interface -----------------------------
// -------------------------------------------------------------------------

static uchar reportBuffer[3] = {0,0,0} ;
static uchar idleRate;           /* in 4 ms units */

const PROGMEM char usbHidReportDescriptor[25] = { /* USB report descriptor */
    0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,                    // USAGE (Consumer Control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x19, 0x00,                    //   USAGE_MINIMUM (Unassigned)
    0x2a, 0x3c, 0x02,              //   USAGE_MAXIMUM (AC Format)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0x3c, 0x02,              //   LOGICAL_MAXIMUM (572)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x10,                    //   REPORT_SIZE (16)
    0x81, 0x00,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};

//Need this empty signature, too lazy to check why
void usbEventResetReady(void){}




/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;

	usbMsgPtr = reportBuffer;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
	if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
	/* we only have one report type, so don't look at wValue */
	DBG1(0x21,rq,8);
	return sizeof(reportBuffer);
}else if(rq->bRequest == USBRQ_HID_GET_IDLE){
	usbMsgPtr = &idleRate;
	DBG1(0x22,rq,8);
	return 1;
}else if(rq->bRequest == USBRQ_HID_SET_IDLE){
	DBG1(0x23,rq,8);
	idleRate = rq->wValue.bytes[1];
	
}else if(rq->bRequest == USBRQ_HID_GET_PROTOCOL){
	DBG1(0x24,rq,8);
	
}else if(rq->bRequest == USBRQ_HID_SET_PROTOCOL){
	DBG1(0x25,rq,8);
}

    }else{
        /* no vendor specific requests implemented */
    }
	return 0;
}
/* ------------------------------------------------------------------------- */


// ------------------------------------------------------------------------- 




#endif /* VUSBCONFIG_H_ */
