#pragma once

#ifdef FREERTOS
#include <timer.hpp>
#include <ticks.hpp>
#endif

namespace timers {


class iTimer {
public:
    iTimer() = default;
    iTimer(uint32_t period) : _period(period) {}

    virtual ~iTimer() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void reset() = 0;
    bool timeout() { return _is_timeout; };

    virtual void set(uint32_t period) {
        _period = period;
    }

protected:
    virtual void _run() { }

    bool _is_timeout = true;
    uint32_t _period = 0;
};


#ifdef FREERTOS
namespace rtos = cpp_freertos;

class OneShotTimer : public iTimer, private rtos::Timer
{
public:
	OneShotTimer() :
	    iTimer(),
	    rtos::Timer(1, false) { }

	OneShotTimer(uint32_t period) :
	    iTimer(period),
	    rtos::Timer(rtos::Ticks::MsToTicks(period), false) { }

	virtual ~OneShotTimer() = default;

	bool timeout()
	{
		return _is_timeout;
	}

	void start()
	{
	    _is_timeout = false;
		Reset();
	}

    void reset()
    {
        start();
    }

	void stop()
	{
		_is_timeout = false;
		Stop();
	}

	void set_period_ms(uint32_t period)
	{
	    _period = period;

		if (IsActive()) {
			stop();
		}

		SetPeriod(rtos::Ticks::MsToTicks(period));
	}

	void set(uint32_t period) {
	    set_period_ms(period);
	}

private:
    void Run()
    {
       _run();
    }

	void _run() {
	    _is_timeout = true;
	}
};
#endif

} /* namespace timers */
