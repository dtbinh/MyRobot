#include "CommPoint.h"
#include "Communicate.h"

using namespace boost::asio;

CommPoint::CommPoint(io_service & io_service, int port, std::string ip)
:comm_channel_(new UdpSession(io_service, port, ip))
{
	
}

CommPoint::~CommPoint()
{

}

int CommPoint::ListenFromAll(typeHandleRead read_callback /* = 0 */)
{
	if (comm_channel_)
	{
		return comm_channel_->ReadFromConnect((unsigned char *)&recv_data_, read_callback);
	}

	return -1;
}

int CommPoint::TalkToAll(std::string msg, int broadcastPort, typeHandleWrite write_callback /* = 0 */, std::string broadcastIP /*= defaultBroadCastAddr*/)
{
	if(comm_channel_)
	{
		return comm_channel_->WriteToRemote(comm_channel_->GenerateBroadcast(broadcastPort, broadcastIP), (const unsigned char *)msg.c_str(), msg.length(), write_callback);
	}

	return -1;
}