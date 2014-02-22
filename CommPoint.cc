#include <boost/bind.hpp>
#include "CommPoint.h"
#include "Communicate.h"

using namespace boost::asio;
using namespace std;

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
		if(read_callback == 0)
		{
			return comm_channel_->ReadFromConnect((unsigned char *)&recv_data_, boost::bind(&CommPoint::handle_read, this, _1, _2, _3));
		}
		else
		{
			return comm_channel_->ReadFromConnect((unsigned char *)&recv_data_, read_callback);
		}
	}

	return -1;
}

int CommPoint::TalkToAll(std::string msg, int broadcastPort, typeHandleWrite write_callback /* = 0 */, std::string broadcastIP /*= defaultBroadCastAddr*/)
{
	if(comm_channel_)
	{
		cout<<"Send msg: " << msg << endl;
		if(write_callback == 0)
		{
			return comm_channel_->WriteToRemote(comm_channel_->GenerateRemote(broadcastPort, broadcastIP), 
				(const unsigned char *)msg.c_str(), 
				msg.length(), 
				boost::bind(&CommPoint::handle_write, this, _1, _2));
		}
		else
		{
			return comm_channel_->WriteToRemote(comm_channel_->GenerateRemote(broadcastPort, broadcastIP), 
				(const unsigned char *)msg.c_str(), 
				msg.length(), 
				write_callback);
		}
	}

	return -1;
}

void CommPoint::handle_read(unsigned char * buf, const boost::system::error_code& error, size_t bytes_transferred)
{
	if(!error)
	{
		cout << buf << endl;
	}
}

void CommPoint::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{
		cout<<bytes_transferred<< " bytes msg Sent" << endl;
	}
}