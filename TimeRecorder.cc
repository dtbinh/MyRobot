#include "TimeRecorder.h"


TimeRecorder::TimeRecorder()
	:t_start(time(NULL)),
	t_end(time(NULL))
{
}


TimeRecorder::~TimeRecorder()
{
}

time_t TimeRecorder::start()
{
	t_start = time(NULL);

	return t_start;
}

time_t TimeRecorder::end()
{
	t_end = time(NULL);

	return t_end;
}

time_t TimeRecorder::getPeriod()
{
	return time(NULL) - t_start;
}

std::string TimeRecorder::getPeriodUnit()
{
	return "seconds";
}
