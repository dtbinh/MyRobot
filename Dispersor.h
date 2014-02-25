#pragma once
#include "LaserRobot.h"

class Dispersor : public LaserRobot
{
public:
	Dispersor(boost::asio::io_service & io_service, std::string host, int player_port);
	~Dispersor();
	
	virtual void Run();
};