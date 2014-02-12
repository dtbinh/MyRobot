#pragma once

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/asio.hpp>

const unsigned short MAX_IP_MTU = 1480;

using boost::asio::ip::udp;

class UdpSession 
{
	UdpSession(boost::asio::io_service io_service, int port, std::string ip);
	~UdpSession();

public:
	int InitRemote(int remotePort, std::string remoteIP);
	int InitBroadcast(int remotePort, std::string broadcastIP);
	void UninitConnect(void);
	int OpenConnect(void);
	int CloseConnect(void);
	int WriteToRemote(const unsigned char * buf,size_t bytes_transferred);
	int WriteToBroadCast(const unsigned char * buf,size_t bytes_transferred);
	bool getConnectAlive(void);

private:
	int StartBind(int localPort, std::string localIP);
	int ReadFromConnect(unsigned char * buf,size_t bytes_transferred = 0);
	void handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error,size_t bytes_transferred);

private:
	enum
	{
		max_length = MAX_IP_MTU,
	};

	boost::asio::io_service & io_service_;
	udp::socket socket_;
	unsigned char recv_data_[max_length];
	
	udp::endpoint remote_endpoint_;
	udp::endpoint broadcast_endpoint_;

};