#pragma once

#include <string>

class TimeRecorder
{
private:
	time_t t_start;
	time_t t_end;
public:
	TimeRecorder();
	~TimeRecorder();

	time_t start();
	time_t end();
	time_t getPeriod();
	std::string getPeriodUnit();
};

