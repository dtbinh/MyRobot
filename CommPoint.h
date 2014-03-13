#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "UdpSession.h"

const std::string startMsg = "start";
const std::string stopMsg = "stop";
const std::string resumeMsg = "resume";
const std::string finishMsg = "finish";
const std::string lineMsg = "line";
const std::string diamondMsg = "diamond";

const std::string defaultBroadCastAddr = "10.0.2.255";

const int locationMsgLen = 10;

class CommPoint
{
public:
	CommPoint(boost::asio::io_service & io_service, int listen_port);
	virtual ~CommPoint(void);

	int ListenFromAll();
	int TalkToAll(std::string msg, int broadcastPort, std::string broadcastIP = defaultBroadCastAddr);

	virtual void ParseRead(unsigned char * buf, size_t bytes_transferred) = 0;

private:
	boost::shared_ptr<UdpSession> comm_channel_;
};