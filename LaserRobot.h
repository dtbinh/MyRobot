#pragma once

#include <libplayerc++/playerc++.h>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include "CommPoint.h"

using namespace PlayerCc;

class LaserRobot :  public CommPoint
{
public:
	LaserRobot(boost::asio::io_service & io_service, int comm_port, std::string host, int player_port);
	~LaserRobot();

	virtual void Run();

private:
	int StopMoving();
	int StartMoving();
	void Resume();
	int LaserAvoidance();
	void handle_timerWalk(const boost::system::error_code& error);

protected:
	double GetXPos();
	double GetYPos();
	void Walk();
	void Stop();

private:
	PlayerClient * robot_;
	LaserProxy * lp_;
	Position2dProxy * pp_;

	boost::asio::deadline_timer timerWalk_;
};