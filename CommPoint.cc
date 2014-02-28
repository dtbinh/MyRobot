#include <boost/bind.hpp>
#include "CommPoint.h"

using namespace boost::asio;
using namespace std;

CommPoint::CommPoint(io_service & io_service, int listen_port)
:comm_channel_(new UdpSession(io_service, listen_port, defaultBroadCastAddr, *this))
{
	
}

CommPoint::~CommPoint()
{

}

int CommPoint::ListenFromAll()
{
	if (comm_channel_)
	{
		return comm_channel_->ReadFromConnect();
	}

	return -1;
}

int CommPoint::TalkToAll(std::string msg, int broadcastPort, std::string broadcastIP /*= defaultBroadCastAddr*/)
{
	if(comm_channel_)
	{
		//cout<<"Send msg: " << msg << endl;
		
		return comm_channel_->WriteToRemote(comm_channel_->GenerateRemote(broadcastPort, broadcastIP), (const unsigned char *)msg.c_str(), msg.length());
	}

	return -1;
}