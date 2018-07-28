// timer.h

#ifndef _TIMER_h
#define _TIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class TimerClass
{
 protected:


 public:
	void init(void (*), int, bool);
	void check();
	void end();
};

//extern TimerClass Timer;
//
#endif

