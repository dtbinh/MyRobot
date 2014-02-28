#pragma once

#include "LaserRobot.h"
#include "CommPoint.h"

class TimeRecorder;

class Leader : public LaserRobot, public CommPoint //, public boost::enable_shared_from_this<Leader>
{
public:
	Leader(boost::asio::io_service & io_service, std::string host, int player_port);
	~Leader();	

	virtual void Run();
	virtual void ParseRead(unsigned char * buf, size_t bytes_transferred);

private:
	void Start(bool bFirstTime);
	void handle_timerCount(const boost::system::error_code& error, bool  bFirstCount);

private:
	boost::asio::deadline_timer timerCount_;
	int countMsg_;
};