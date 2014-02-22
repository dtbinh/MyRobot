#pragma once

#include "LaserRobot.h"

class TimeRecorder;

class Leader : public LaserRobot
{
public:
	Leader(boost::asio::io_service & io_service, std::string host, int player_port);
	~Leader();	

	virtual void Run();

private:
	void Start(bool bFirstTime);
	void handle_timerCount(const boost::system::error_code& error, bool  bFirstCount);
	void handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred);

private:
	boost::asio::deadline_timer timerCount_;
	int countMsg_;
};