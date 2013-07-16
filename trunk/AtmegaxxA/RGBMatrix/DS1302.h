#include <stdint.h>
/*
 * DS1302.h
 *
 * Created: 2013/03/26 21:15:12
 *  Author: Alan
 */ 


#ifndef DS1302_H_
#define DS1302_H_

// Structure for the first 8 registers.
// These 8 bytes can be read at once with
// the 'clock burst' command.
// Note that this structure contains an anonymous union.
// It might cause a problem on other compilers.
typedef struct ds1302_struct
{
	uint8_t Seconds:4;      // low decimal digit 0-9
	uint8_t Seconds10:3;    // high decimal digit 0-5
	uint8_t CH:1;           // CH = Clock Halt
	uint8_t Minutes:4;
	uint8_t Minutes10:3;
	uint8_t reserved1:1;
	union
	{
		struct
		{
			uint8_t Hour:4;
			uint8_t Hour10:2;
			uint8_t reserved2:1;
			uint8_t hour_12_24:1; // 0 for 24 hour format
		} h24;
		struct
		{
			uint8_t Hour:4;
			uint8_t Hour10:1;
			uint8_t AM_PM:1;      // 0 for AM, 1 for PM
			uint8_t reserved2:1;
			uint8_t hour_12_24:1; // 1 for 12 hour format
		} h12;
	};
	uint8_t Date:4;           // Day of month, 1 = first day
	uint8_t Date10:2;
	uint8_t reserved3:2;
	uint8_t Month:4;          // Month, 1 = January
	uint8_t Month10:1;
	uint8_t reserved4:3;
	uint8_t Day:3;            // Day of week, 1 = first day (any day)
	uint8_t reserved5:5;
	uint8_t Year:4;           // Year, 0 = year 2000
	uint8_t Year10:4;
	uint8_t reserved6:7;
	uint8_t WP:1;             // WP = Write Protect
};


void setupDS1302();
void DS1302_clock_burst_read( uint8_t *p);
void DS1302_clock_burst_write( uint8_t *p);

#endif /* DS1302_H_ */