#pragma once

#include <boost/asio.hpp>

class CommPoint
{
public:
	CommPoint(boost::asio::io_service & io_service);
	virtual ~CommPoint(void);

	int CreateListen(int port, int index);
	int CreateBroadcast(int port, int index);
	int ListenFromAll(int id, char * msg);
	int TalkToAll(int id, int index, char * msg);

private:
	boost::asio::io_service & io_service_;
};