#pragma once

#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

typedef boost::function<void (unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred)> typeHandleRead;
typedef boost::function<void (const boost::system::error_code& error,size_t bytes_transferred)> typeHandleWrite;

const int max_msg_len = 256;

using boost::asio::ip::udp;

class UdpSession 
{
public:
	UdpSession(boost::asio::io_service & io_service, int port, std::string ip);
	~UdpSession();

	int OpenConnect(void);
	int CloseConnect(void);
	
	udp::endpoint GenerateRemote(int remotePort, std::string remoteIP);
	
	int WriteToRemote(udp::endpoint remote_endpoint, const unsigned char * buf, size_t bytes_transferred, typeHandleWrite write_callback);
	int ReadFromConnect( unsigned char * buf, typeHandleRead read_callback, size_t bytes_transferred = 0);
	
	bool getConnectAlive(void);

private:	
	void handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error,size_t bytes_transferred);

private:
	boost::asio::io_service & io_service_;
	boost::asio::ip::udp::udp::socket socket_;

	unsigned char recv_data_[max_msg_len];

	//typeHandleRead after_read;
	//typeHandleWrite after_write;
};