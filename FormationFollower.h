#pragma once
#include "LaserRobot.h"
#include "CommPoint.h"
#include "Coordinate.h"

class FormationFollower : public LaserRobot, public CommPoint
{
public:
	FormationFollower(boost::asio::io_service & io_service, std::string host, int player_port);
	~FormationFollower();

	void ParseRead(unsigned char * buf, size_t bytes_transferred);

private:
	void ParseMessage(std::string msg);
	void Movement(std::string formationType, CoorPtr leadr);
	
};