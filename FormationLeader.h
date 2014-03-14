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
	void Run();

private:
	void BroadcastLocation(std::string formationType, double x_pos, double y_pos);
	void NotifyManager();
	void ParseMessage(std::string msg);
	void Move(std::string formationType, Coordinate & goal);
	void handle_timerWalk(const boost::system::error_code& error, bool bProcessingTask);

	void Resume(bool bProcessingTask);

private:
	std::string formationMsg_;
	Coordinate goal_;
	boost::asio::deadline_timer timerWalk_;
};