#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "External Libraries/SDL/include/SDL_stdinc.h"
class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Resume();

	bool isStoped() { return !running; };

	Uint32 Read();
	float ReadSec();

private:

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
};

#endif //__TIMER_H__