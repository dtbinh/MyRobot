#pragma once
#include "CommPoint.h"
#include <queue>
#include "Coordinate.h"

struct stSection
{
	std::string formationType_;
	Coordinate waypoint_;

	stSection(std::string type, Coordinate coor)
	:formationType_(type),
	waypoint_(coor)
	{
	}
};

class Manager : public CommPoint
{
public:
	Manager(boost::asio::io_service & io_service);
	~Manager();

	void ParseRead(unsigned char * buf, size_t bytes_transferred);
	void Run();

	int AddWayPoint(Coordinate waypoint);
	int AddWayPoint(double x, double y);

private:
	void SendCmd();
	void RecordTime();
	void Report();

private:
	std::queue<stSection> waypoints_;
	boost::posix_time::ptime curPt_;	
};