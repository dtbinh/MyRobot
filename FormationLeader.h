#pragma once
#include "LaserRobot.h"
#include "CommPoint.h"
#include "Coordinate.h"

class FormationLeader : public LaserRobot, public CommPoint
{
public:
	FormationLeader(boost::asio::io_service & io_service, std::string host, int player_port);
	~FormationLeader();

	void ParseRead(unsigned char * buf, size_t bytes_transferred);	

private:
	void ParseMessage(std::string msg);
	void BroadcastLocation(std::string formationType, double x_pos, double y_pos);
	void Movement(std::string formationType, CoorPtr goal);

private:
	std::string formationMsg_;
};