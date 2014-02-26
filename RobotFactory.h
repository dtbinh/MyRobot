#pragma once
#include <boost/asio.hpp>
#include "Robot.h"

const int robotLeader = 1;
const int robotFollower = 2;
const int robotAggregator = 3;
const int robotDispersor = 4;

class RobotFactory
{
private:
	RobotFactory();

public:
	static Robot * CreateRobot(int type, boost::asio::io_service & io_service, std::string host, int player_port);
	~RobotFactory();
	
};