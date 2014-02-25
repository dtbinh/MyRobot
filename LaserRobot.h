#pragma once

#include <libplayerc++/playerc++.h>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/asio.hpp>
#include "Robot.h"

using namespace PlayerCc;

class LaserRobot : public Robot
{
public:
	LaserRobot(boost::asio::io_service & io_service, std::string host, int player_port);
	~LaserRobot();

	virtual void Run();

protected:
	double GetXPos();
	double GetYPos();
	double GetYaw();
	void SetSpeed(double forwardSpeed, double turnSpeed);
	
	void Walk();
	void Stop();
	
	void LaserAvoidance();
	
private:
	int StopMoving();
	int StartMoving();
	void Resume();
	void handle_timerWalk(const boost::system::error_code& error);

private:
	PlayerClient * robot_;
	LaserProxy * lp_;
	Position2dProxy * pp_;

	boost::asio::deadline_timer timerWalk_;
};