#pragma once
#include <chrono>
class SoupTimer
{
public:
	SoupTimer();

	// Returns the time since this was last called
	float Mark();

	// Returns the time since mark was called without resetting
	float Peek() const;

private:
	std::chrono::steady_clock::time_point last;
};

