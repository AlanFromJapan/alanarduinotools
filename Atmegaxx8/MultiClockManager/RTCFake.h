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

#define SEC_IN_ONE_DAY (60L * 60L * 24L)

uint32_t mRtcFakeSecondCounter = 0L;
uint32_t mRtcFakeSpeedFactor = 10L;
uint32_t mRtcFakeSpeedDivider = 1L;

uint32_t mRtcFakeSubCounter = 0L;

void ReadTimeFake(Date* pTimeDate){
	mRtcFakeSubCounter++;
	if (mRtcFakeSubCounter >= mRtcFakeSpeedDivider){
		mRtcFakeSecondCounter += mRtcFakeSpeedFactor;
		mRtcFakeSubCounter = 0;
	}
	
	if (mRtcFakeSecondCounter >= SEC_IN_ONE_DAY){
		mRtcFakeSecondCounter = 0;
	}
	
	(*pTimeDate).second = (uint8_t)((mRtcFakeSecondCounter % 60));
	(*pTimeDate).minute = (uint8_t)((mRtcFakeSecondCounter / 60));
	(*pTimeDate).hour = (uint8_t)((mRtcFakeSecondCounter) / 3600);
	//ignore the rest...
	
}	


#endif /* RTCFAKE_H_ */