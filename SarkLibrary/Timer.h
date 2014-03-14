#ifndef __TIMER_H__
#define __TIMER_H__

#include "core.h"
#include <chrono>

namespace sarklib{

	// timer's time unit is 'second' type as real_d
	// timer can be the real-time timer or fixed-flow timer
	class Timer{
	private:
		typedef std::chrono::steady_clock Clock;
		typedef Clock::time_point Time;
		// time duration type of steady_clock is uint64_t in visual studio 13
		
		Time _start, _prev, _cur, _pause;

		real_d mElapsedTime;
		real_d mDeltaTime;

		bool mWorking;

	public:
		Timer(bool createWorking = false);
		~Timer();

		// update the timer. if it is not working now, then it'll fail
		bool Update();

		// pause the timer. if it is paused already, then it'll fail
		bool Pause();

		// resume the timer. if it is working now, then it'll fail
		bool Resume();

		// is this timer working?
		bool IsWorking() const;

		// get currently elapsed time of engine running
		const real_d& GetElapsedTime() const;

		// get delta time
		const real_d& GetDeltaTime() const;
	};

}
#endif