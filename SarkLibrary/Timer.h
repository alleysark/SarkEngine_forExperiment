#ifndef __TIMER_H__
#define __TIMER_H__

#include "core.h"
#include <chrono>

namespace sark{

	// timer's time unit is 'second' type as real
	// timer can be the real-time timer or fixed-flow timer
	class Timer{
	private:
		typedef std::chrono::steady_clock Clock;
		typedef Clock::time_point Time;
		// time duration type of steady_clock is uint64_t in visual studio 13
		
		Time _start, _prev, _cur, _pause;

		real mElapsedTime;
		real mDeltaTime;

		bool mWorking;

		bool mFixedFlow;

		real mFixedDeltaTime;

	public:
		Timer(bool createWorking = false,
			bool fixedFlow = false, real fixedDeltaTime = 1.f / 60.f);
		~Timer();

		// update the timer. if it is not working now, then it'll fail
		bool Update();

		// pause the timer. if it is paused already, then it'll fail
		bool Pause();

		// resume the timer. if it is working now, then it'll fail
		bool Resume();

		// is this timer working?
		bool IsWorking() const;

		// is this timer fixed-flow?
		bool IsFixedFlow() const;
		// set timer fixed flow or not.
		void FixateFlow(bool on);

		// set fixed-flow delta time.
		void SetFixedDeltaTime(real delta);

		// get currently elapsed time of engine running
		const real& GetElapsedTime() const;

		// get delta time
		const real& GetDeltaTime() const;
	};

}
#endif