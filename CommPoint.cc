#include <boost/bind.hpp>
#include "CommPoint.h"
#include "Communicate.h"

using namespace boost::asio;
using namespace std;

CommPoint::CommPoint(io_service & io_service, int listen_port)
:comm_channel_(new UdpSession(io_service, listen_port, defaultBroadCastAddr))
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
		cout<<"Send msg: " << msg << endl;
		
		return comm_channel_->WriteToRemote(comm_channel_->GenerateRemote(broadcastPort, broadcastIP), (const unsigned char *)msg.c_str(), msg.length());
	}

	return -1;
}

void CommPoint::handle_read(unsigned char * buf, const boost::system::error_code& error, size_t bytes_transferred)
{
	if(!error)
	{
		cout <<"CommPoint Recv Msg: "<< string(buf, buf + bytes_transferred) << endl;
	}
}

void CommPoint::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		cout<<bytes_transferred<< " bytes msg Sent" << endl;
	}
}