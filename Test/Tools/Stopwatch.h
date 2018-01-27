#pragma once
#include <iostream>
#include <chrono>
#include "Core.h"

namespace testTool
{

using namespace std::chrono;

class Stopwatch
{
public:
	Stopwatch() = default;
	Stopwatch(const Stopwatch&) = delete;
	~Stopwatch() = default;

	void start()
	{
		m_timestamp = system_clock::now();
	}

	void stop()
	{
		auto l_elapsedTime = system_clock::now() - m_timestamp;
		m_elapsedTime = duration_cast<milliseconds>(l_elapsedTime);
	}

	milliseconds getElapsedTime() const
	{
		return m_elapsedTime;
	}

public:
	system_clock::time_point m_timestamp;
	milliseconds m_elapsedTime;
};




}