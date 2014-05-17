#include "Timer.h"


namespace sark{

	Timer::Timer(bool createWorking, bool fixedFlow, real fixedDeltaTime)
		: mElapsedTime(0.f), mDeltaTime(0.f), mWorking(createWorking),
		mFixedFlow(fixedFlow), mFixedDeltaTime(fixedDeltaTime)
	{
		_start = Clock::now();
		_prev = _start;

		if ( !mWorking)
			_pause = _start;
	}

	Timer::~Timer(){
	}

	// update the timer. if it is not working now, then it'll fail
	bool Timer::Update(){
		if (!mWorking)
			return false;

		if (mFixedFlow){
			mElapsedTime += mFixedDeltaTime;
			mDeltaTime = mFixedDeltaTime;
		}
		else{
			_cur = Clock::now();
			mElapsedTime = static_cast<real>(real_d((_cur - _start).count()) * Clock::period::num / Clock::period::den);
			mDeltaTime = static_cast<real>(real_d((_cur - _prev).count()) * Clock::period::num / Clock::period::den);
			_prev = _cur;
		}
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

	// is this timer fixed-flow?
	bool Timer::IsFixedFlow() const{
		return mFixedFlow;
	}
	// set timer fixed flow or not.
	void Timer::FixateFlow(bool on){
		mFixedFlow = on;
	}

	// set fixed-flow delta time.
	void Timer::SetFixedDeltaTime(real delta){
		mFixedDeltaTime = delta;
	}

	// get currently elapsed time of engine running
	const real& Timer::GetElapsedTime() const{
		return mElapsedTime;
	}

	// get delta time
	const real& Timer::GetDeltaTime() const{
		return mDeltaTime;
	}

}