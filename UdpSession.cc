#include "UdpSession.h"
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include "CommPoint.h"

using namespace std;

UdpSession::UdpSession(boost::asio::io_service & io_service, int port, string ip, CommPoint & cp)
:io_service_(io_service),
socket_(io_service),
commp_(cp)
{
	boost::asio::ip::udp::endpoint listen_endpoint(boost::asio::ip::address::from_string(ip), port);
    socket_.open(listen_endpoint.protocol());

	socket_.set_option(boost::asio::socket_base::reuse_address(true));
	socket_.set_option(boost::asio::socket_base::broadcast(true));

	socket_.bind(listen_endpoint);

}

UdpSession::~UdpSession() 
{
	if (getConnectAlive())
	{
		CloseConnect();
	}
}

udp::endpoint UdpSession::GenerateRemote(int remotePort, string remoteIP)
{
	udp::resolver resolver(io_service_);
	udp::resolver::query query(udp::v4(), remoteIP, boost::lexical_cast<std::string>(remotePort));
	
	boost::system::error_code error;
	return *resolver.resolve(query,error);
}

int UdpSession::OpenConnect(void)
{
	return 0;
}

int UdpSession::CloseConnect(void)
{
	boost::system::error_code ec;
	socket_.cancel(ec);
	socket_.shutdown(udp::socket::shutdown_both,ec);

	socket_.close();

	return 0;
}

int UdpSession::ReadFromConnect()
{
	if(!getConnectAlive())
	{
		return -1;
	}

	udp::endpoint remote_endpoint;

	socket_.async_receive_from(boost::asio::buffer(recv_data_,max_msg_len),remote_endpoint,
		boost::bind(&UdpSession::handle_read,
		shared_from_this(),
		recv_data_,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	return 0;
}

int UdpSession::WriteToRemote(udp::endpoint remote_endpoint, const unsigned char * buf, size_t bytes_transferred)
{
	if(!getConnectAlive())
	{
		return -1;
	}

	socket_.async_send_to(boost::asio::buffer(buf, bytes_transferred),remote_endpoint,
		boost::bind(&UdpSession::handle_write,
		this, //shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	return 0;
}

void UdpSession::handle_read(unsigned char * buf, const boost::system::error_code& error, size_t bytes_transferred)
{
	if(!error)
	{
		//cout<<"UdpSession recv msg: " << buf << endl;
		commp_.ParseRead(buf,bytes_transferred);
	}
	else
	{
		cout<<"UdpSession read error"<<endl;
	}

	ReadFromConnect();
}

void UdpSession::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
{
	if(!error)
	{
		//cout<<bytes_transferred<<" bytes sent"<<endl;
	}
	else 
	{
		cout<<"UdpSession write error"<<endl;	
	}
}

bool UdpSession::getConnectAlive(void)
{
	return socket_.is_open();
}

