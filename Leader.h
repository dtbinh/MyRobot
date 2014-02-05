#pragma once

#include "LaserRobot.h"

class TimeRecorder;

class Leader : public LaserRobot
{
private:
	char msg[max_msg_len];

private:
	void Start();
	void Resume();
	void Walk(TimeRecorder & tr, bool bFirst);
	void Listen(int countCatchMsgNum);

public:
	Leader(std::string host, int port);
	~Leader();	

	virtual void Run();
};