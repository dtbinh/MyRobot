#include "UdpSession.h"
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

using namespace std;

UdpSession::UdpSession(boost::asio::io_service io_service, int port, string ip)
:io_service_(io_service),
socket_(io_service_)
{
	StartBind(port, ip);
}

UdpSession::~UdpSession() 
{
	if (getConnectAlive())
	{
		CloseConnect();
	}

	UninitConnect();
}

int UdpSession::StartBind(int localPort, string localIP)
{
	boost::system::error_code ec;

	if (localIP.empty())
	{
		socket_.open(udp::v4());
		socket_.bind(udp::endpoint(udp::v4(),localPort));
	}
	else 
	{
		socket_.open(udp::v4());
		socket_.bind(udp::endpoint(boost::asio::ip::address::from_string(localIP),localPort),ec);	
	}

	return 0;
}

int UdpSession::InitRemote(int remotePort, string remoteIP)
{
	udp::resolver resolver(io_service_);
	udp::resolver::query query(udp::v4(), remoteIP, boost::lexical_cast<std::string>(remotePort));
	
	boost::system::error_code error;
	remote_endpoint_ = *resolver.resolve(query,error);
}

int UdpSession::InitBroadcast(int remotePort, string broadcastIP)
{
	broadcast_endpoint_ = udp::endpoint(boost::asio::ip::address::from_string(broadcastIP),remotePort);
}

void UdpSession::UninitConnect(void)
{
	CloseConnect();
}

int UdpSession::OpenConnect(void)
{
	
}

int UdpSession::CloseConnect(void)
{
	boost::system::error_code ec;
	socket_.cancel(ec);
	socket_.shutdown(udp::socket::shutdown_both,ec);

	socket_.close();

	return 0;
}

int UdpSession::ReadFromConnect(unsigned char * buf,size_t bytes_transferred /*= 0*/)
{
	udp::endpoint remote_endpoint;

	socket_.async_receive_from(boost::asio::buffer(buf,max_length),remote_endpoint,
		boost::bind(&UdpSession::handle_read,
		this,
		buf,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	return 0;
}

int UdpSession::WriteToBroadCast(const unsigned char * buf,size_t bytes_transferred)
{
	boost::asio::socket_base::broadcast option(true);
	socket_.set_option(option);

	socket_.async_send_to(boost::asio::buffer(buf, bytes_transferred),broadcast_endpoint_,
		boost::bind(&UdpSession::handle_write,
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	return 0;
}

int UdpSession::WriteToRemote(const unsigned char * buf,size_t bytes_transferred)
{
	boost::asio::socket_base::broadcast option(true);
	socket_.set_option(option);

	socket_.async_send_to(boost::asio::buffer(buf, bytes_transferred),remote_endpoint_,
		boost::bind(&UdpSession::handle_write,
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	return 0;
}

void UdpSession::handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred)
{
	if(!error)
	{
		
	}
	else
	{
		CloseConnect();
		UninitConnect();
		OpenConnect();
	}
}

void UdpSession::handle_write(const boost::system::error_code& error,size_t bytes_transferred)
{
}

bool UdpSession::getConnectAlive(void)
{
	return socket_.is_open();
}

