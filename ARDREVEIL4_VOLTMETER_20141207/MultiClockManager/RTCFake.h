/*
 * RTCFake.h
 *
 * Created: 2014/11/23 12:17:16
 *  Author: Alan
 */ 


#ifndef RTCFAKE_H_
#define RTCFAKE_H_


#define RTC_FAKE

uint32_t mRtcFakeSecondCounter = 0L;
uint32_t mRtcFakeSpeedFactor = 1L;
uint32_t mRtcFakeSpeedDivider = 200L;

void ReadTimeFake(Date* pTimeDate){
	mRtcFakeSecondCounter += mRtcFakeSpeedFactor;
	
	(*pTimeDate).second = 0;
	(*pTimeDate).minute = (uint8_t)((mRtcFakeSecondCounter / mRtcFakeSpeedDivider) % 60);
	(*pTimeDate).hour = (uint8_t)((mRtcFakeSecondCounter / mRtcFakeSpeedDivider) / 60);
	//ignore the rest...
}	


#endif /* RTCFAKE_H_ */