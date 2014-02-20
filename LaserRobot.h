#pragma once

#include <libplayerc++/playerc++.h>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include "CommPoint.h"

using namespace PlayerCc;

const std::string startMsg = "start";
const std::string stopMsg = "stop";
const std::string resumeMsg = "resume";

const int max_msg_len = 256;
const int locationMsgLen = 11;

class LaserRobot :  public CommPoint
{
private:
	PlayerClient * robot;
	LaserProxy * lp;

protected:
	std::string myHost;
	int myPort;
	Position2dProxy * pp;

protected:
	int listenID;
	int broadcastID;

protected:
	int LaserAvoidance();
	int StopMoving();
	int StartMoving();

public:
	LaserRobot(boost::asio::io_service & io_service, std::string host, int port);
	~LaserRobot();

	virtual void Run();
};