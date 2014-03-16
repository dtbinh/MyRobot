#pragma once

#include <libplayerc++/playerc++.h>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/asio.hpp>
#include "Robot.h"

using namespace PlayerCc;

const int comm_interval = 1; // milliseconds

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

	double GetXSpeed();
	double GetYSpeed();
	double GetYawSpeed();
	
	void SetSpeed(double xSpeed, double turnSpeed);
	void SetSpeed(double xSpeed, double ySpeed, double turnSpeed);

	void SetVelHead(double xSpeed, double yawHead);
	
	void GoTo(double x_pos, double y_pos);
	void GoTo(player_pose2d_t pos, player_pose2d_t speed);

	void Walk();
	void Stop();
	
	void Read();
	void Resume();
	void LaserAvoidance(double & newspeed, double & newturnrate);
	
private:
	void handle_timerWalk(const boost::system::error_code& error);

private:
	PlayerClient * robot_;
	LaserProxy * lp_;
	Position2dProxy * pp_;

	boost::asio::deadline_timer timerWalk_;
};