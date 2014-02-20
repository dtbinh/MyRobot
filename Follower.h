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
	Follower(boost::asio::io_service & io_service, std::string host, int port);
	~Follower();

	virtual void Run();
	
};