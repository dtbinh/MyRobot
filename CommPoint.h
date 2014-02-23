#pragma once

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include "UdpSession.h"

const std::string startMsg = "start";
const std::string stopMsg = "stop";
const std::string resumeMsg = "resume";

const int locationMsgLen = 10;

class CommPoint
{
public:
	CommPoint(boost::asio::io_service & io_service, int port, std::string ip);
	virtual ~CommPoint(void);

	int ListenFromAll(typeHandleRead read_callback = 0);
	int TalkToAll(std::string msg, int broadcastPort, typeHandleWrite write_callback = 0, std::string broadcastIP = defaultBroadCastAddr);

private:	
	void handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error,size_t bytes_transferred);

protected:
	unsigned char recv_data_[max_msg_len];
	
private:
	boost::scoped_ptr<UdpSession> comm_channel_;
};