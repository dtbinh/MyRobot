#pragma once
#include "LaserRobot"

class FormationFollower
{
public:
	FormationFollower(boost::asio::io_service & io_service, std::string host, int player_port);
	~FormationFollower();
	
};