#pragma once

#include "LaserRobot.h"

class Follower : public LaserRobot
{
public:
	Follower(boost::asio::io_service & io_service, std::string host, int player_port);
	~Follower();

	virtual void Run();

private:
	int handle_read(const unsigned char * buf, size_t bytes_transferred);
	void SendLocation();
	bool ParseMsg(const char * msg);

private:
	int myPort_;
};