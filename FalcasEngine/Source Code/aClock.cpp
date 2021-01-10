#include "aClock.h"

aClock Time::realTimer;
aClock Time::gameTimer;

int Time::numFrames = 0;

aClock::aClock() :timeScale(1.0f), paused(false)
{

}

void aClock::Start()
{
	active = true;
	paused = false;
	started = true;
	stopped = false;
	timeScale = 1.0f;
	timer.Start();
}

void aClock::Stop()
{
	paused = false;
	started = false;
	stopped = true;
	active = false;
}

void aClock::Pause()
{
	paused = true;
	timeScale = 0.0f;
	timer.Stop();
	active = false;
}

void aClock::Resume()
{
	paused = false;
	timeScale = 1.0f;
	timer.Resume();
	active = true;
}

void aClock::Reset()
{
	timeScale = 1.0f;
	paused = false;
	stopped = false;
	active = true;
}

void aClock::Step()
{
	dt = (float)deltaTimer.Read() / 1000 * timeScale;
	deltaTimer.Start();
}

float aClock::totalTime()
{
	if (started)
		return timer.ReadSec();
	else
		return 0.0f;
}

float aClock::deltaTime()
{
	return dt * timeScale;
}

void Time::Init()
{
	gameTimer.timeScale = 0.0f;
	gameTimer.started = false;
	gameTimer.paused = true;
	gameTimer.stopped = true;
	gameTimer.active = false;

	realTimer.timer.Start();
	realTimer.started = true;
	realTimer.paused = false;
	realTimer.stopped = false;
}
