#pragma once
#include "CommPoint.h"
#include <queue>

class Manager : public CommPoint
{
public:
	Manager(boost::asio::io_service & io_service);
	~Manager();

private:
	std::queue<Coordinate> waypoints_;
	
};