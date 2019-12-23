// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

void Timer::Reset()
{
	started_at = 0;
	stopped_at = 0;
}

// ---------------------------------------------
void Timer::Stop()
{
	stopped_at = SDL_GetTicks();
	running = false;
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

float Timer::ReadSec() const
{
	if (running == true)
	{
		return (SDL_GetTicks() - started_at) / 1000.0f;
	}
	else
	{
		return (stopped_at - started_at) / 1000.0f;
	}
}


