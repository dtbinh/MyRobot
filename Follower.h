#pragma once

#include "LaserRobot.h"
#include "CommPoint.h"

class Follower : public LaserRobot, public CommPoint
{
public:
	Follower(boost::asio::io_service & io_service, std::string host, int player_port);
	~Follower();

	virtual void Run();

private:
	void handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred);
	void SendLocation();
	bool ParseMsg(const unsigned char * msg, size_t length);

private:
	int myPort_;
};