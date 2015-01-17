/*
 * RTCFake.h
 *
 * Created: 2014/11/23 12:17:16
 *  Author: Alan
 */ 


#ifndef RTCFAKE_H_
#define RTCFAKE_H_


#define RTC_FAKE

#ifdef RTC_FAKE
	#define RTC_READ_TIME(p)	ReadTimeFake(p)
	#define RTC_INIT()			;
	#define RTC_SET_TIME(p)		;	
#endif


uint32_t mRtcFakeSecondCounter = 0L;
uint32_t mRtcFakeSpeedFactor = 10L;
uint32_t mRtcFakeSpeedDivider = 200L;

void ReadTimeFake(Date* pTimeDate){
	mRtcFakeSecondCounter += mRtcFakeSpeedFactor;
	
	(*pTimeDate).second = 0;
	(*pTimeDate).minute = (uint8_t)((mRtcFakeSecondCounter / mRtcFakeSpeedDivider) % 60);
	(*pTimeDate).hour = (uint8_t)((mRtcFakeSecondCounter / mRtcFakeSpeedDivider) / 60);
	//ignore the rest...
}	


#endif /* RTCFAKE_H_ */