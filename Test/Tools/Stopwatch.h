#pragma once
#include <iostream>
#include <chrono>

namespace testTool
{

class Stopwatch
{
public:
	Stopwatch() = default;
	Stopwatch(const Stopwatch&) = delete;
	~Stopwatch() = default;

	void start()
	{

	}

	void stop()
	{

	}

	std::chrono::milliseconds getElapsedTime() const
	{

	}

public:
	std::chrono::microseconds m_timestamp;
	std::chrono::milliseconds m_elapsedTime;
};




}