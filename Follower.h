#pragma once

#include "LaserRobot.h"

class Follower : public LaserRobot
{
private:
	char msg[max_msg_len];

private:
	int SendLocation();
	bool ParseMsg(char * msg);

public:
	Follower(std::string host, int port);
	~Follower();

	virtual void Run();
	
};