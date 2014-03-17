#pragma once
#include <boost/unordered_map.hpp>
#include "LaserRobot.h"
#include "CommPoint.h"
#include "Formation.h"

class FormationFollower : public LaserRobot, public CommPoint
{
public:
	FormationFollower(boost::asio::io_service & io_service, std::string host, int player_port);
	~FormationFollower();

	void ParseRead(unsigned char * buf, size_t bytes_transferred);
	void Run();

private:
	void BroadcastLocation(double x_pos, double y_pos);
	void ParseMessage(std::string msg);
	void Movement(std::string formationType, CoorPtr leaderPos, double leaderSpeed, double leaderYaw);
	FormationPtr getFormation(std::string type);
	size_t Port2Index(int port);

private:
	int MyPort_;
	boost::unordered_map<std::string, FormationPtr> formations_;
};