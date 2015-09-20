#ifndef __INIT_H__
#define __INIT_H__

typedef enum
{
	SUCCESS,
	INIT_FAILURE
} statusRtn;

statusRtn init(void);

	//Name this something that makes sense when you write it in code
#define PERIOD_500MS 0xC2F6
#define PERIOD_10MS  0xF3C7
#define PERIOD_100MS 0xF3CA
#define PERIOD_250MS 0xE17B
#define PERIOD_800HZ 0xFFD9

#define TIMER1_PERIOD PERIOD_800HZ

#endif