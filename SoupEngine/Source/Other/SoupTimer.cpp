#include "SoupTimer.h"

SoupTimer::SoupTimer()
{
	last = std::chrono::steady_clock::now();
}

float SoupTimer::Mark()
{
	const std::chrono::steady_clock::time_point old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
	return frameTime.count();
}

float SoupTimer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}