#pragma once
#include "LaserRobot.h"

class FormationLeader : LaserRobot
{
public:
	FormationLeader(boost::asio::io_service & io_service, std::string host, int player_port);
	~FormationLeader();
	
};