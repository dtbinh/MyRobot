#pragma once
#include <boost/unordered_map.hpp>
#include "LaserRobot.h"
#include "CommPoint.h"
#include "Coordinate.h"
#include "Formation.h"

class FormationLeader : public LaserRobot, public CommPoint
{
public:
	FormationLeader(boost::asio::io_service & io_service, std::string host, int player_port);
	~FormationLeader();

	void ParseRead(unsigned char * buf, size_t bytes_transferred);	
	void Run();

private:
	void BroadcastLocation(std::string formationType, double x_pos, double y_pos, double speed, double yaw);
	void NotifyManager();
	void ParseMessage(std::string msg);
	void Move(std::string formationType, Coordinate & goal);
	void handle_timerWalk(const boost::system::error_code& error, bool bProcessingTask);
	void AdjustSpeed(int port, CoorPtr follower);
	FormationPtr getFormation(std::string type);
	size_t Port2Index(int port);

	void Resume(bool bProcessingTask);

	void Speeder(double limitation);
	void Slowdown(double limitation);

private:
	double myspeed_;
	Coordinate goal_;
	std::string formationMsg_;
	boost::asio::deadline_timer timerWalk_;
	boost::unordered_map<std::string, FormationPtr> formations_;
};