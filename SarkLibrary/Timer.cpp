#include "Timer.h"


namespace sark{

	Timer::Timer(bool createWorking){
		_start = Clock::now();
		_prev = _start;

		mElapsedTime = 0;
		mDeltaTime = 0;

		mWorking = createWorking;
		if ( !mWorking)
			_pause = _start;
	}

	Timer::~Timer(){
	}

	// update the timer. if it is not working now, then it'll fail
	bool Timer::Update(){
		if (!mWorking)
			return false;

		_cur = Clock::now();
		mElapsedTime = real_d((_cur - _start).count()) * Clock::period::num / Clock::period::den;
		mDeltaTime = real_d((_cur - _prev).count()) * Clock::period::num / Clock::period::den;
		_prev = _cur;
		
		return true;
	}

	// pause the timer. if it is paused already, then it'll fail
	bool Timer::Pause(){
		if (!mWorking)
			return false;

		mWorking = false;
		_pause = Clock::now();
		return true;
	}

	// resume the timer. if it is working now, then it'll fail
	bool Timer::Resume(){
		if (mWorking)
			return false;

		_cur = Clock::now();

		// shrink start and previous time for the paused time;
		Clock::duration dur = _cur - _pause;
		_start += dur;
		_prev += dur;

		mWorking = true;
		return true;
	}

	// is this timer working?
	bool Timer::IsWorking() const{
		return mWorking;
	}

	// get currently elapsed time of engine running
	const real_d& Timer::GetElapsedTime() const{
		return mElapsedTime;
	}

	// get delta time
	const real_d& Timer::GetDeltaTime() const{
		return mDeltaTime;
	}

}