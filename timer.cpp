// 
// 
// 

#include "timer.h"
long timer;

void TimerClass::init(void *callBack, int pulse_ms, bool repeat)
{
	timer = millis();
}

void TimerClass::check()
{
}

void TimerClass::end()
{
}
