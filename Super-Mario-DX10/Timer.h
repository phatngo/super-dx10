#pragma once
#include <d3d10.h>
class Timer
{
private:
	ULONGLONG timeStart;
	bool started;
public:
	Timer();

	bool IsStarted() { return this->started; }
	void Start();
	void Reset();
	ULONGLONG ElapsedTime();
};

