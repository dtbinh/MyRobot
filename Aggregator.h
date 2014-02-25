#pragma once
#include "Centralization.h"

class Aggregator : public Centralization
{
public:
	Aggregator(boost::asio::io_service & io_service, std::string host, int player_port);
	~Aggregator();

	virtual bool CompareToInterRobot(CoorPtr other);
	virtual bool ComapreToCenter(CoorPtr center);
	virtual void Moving(CoorPtr destination);
	virtual void Identify();

};