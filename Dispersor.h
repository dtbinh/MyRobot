#pragma once
#include "Centralization.h"

class Dispersor : public Centralization
{
public:
	Dispersor(boost::asio::io_service & io_service, std::string host, int player_port);
	~Dispersor();

	virtual bool CompareToInterRobot(CoorPtr other);
	virtual bool ComapreToCenter(CoorPtr center);
	virtual void Moving(CoorPtr source);
	virtual void Identify();
};