#pragma once
#ifndef _CLOCK_
#define _CLOCK_

#include "Timer.h"

struct aClock {
	aClock();
	void Start();
	void Stop();
	void Pause();
	void Resume();
	void Reset();
	void Step();


	float totalTime();

	float deltaTime(); 
	float timeScale;   
	bool paused;
	bool started;
	bool stopped;
	float dt;

	Timer timer;
	Timer deltaTimer;
};

class Time {
public:
	static void Init();
	static int numFrames;

	static aClock realTimer;
	static aClock gameTimer;
};

#endif // !_CLOCK_
