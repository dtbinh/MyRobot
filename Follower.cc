#include "Follower.h"

using namespace std;

const int defaultListenPort = 9001;
const int defautBroadCastPort = 9000;

Follower::Follower(boost::asio::io_service & io_service, string host, int player_port)
:LaserRobot(io_service, defaultListenPort, host, player_port)
{
	myPort_ = player_port;
}

Follower::~Follower()
{
	
}

void Follower::SendLocation()
{
	int x_pos = GetXPos();
	int y_pos = GetYPos();

	ostringstream msg;
	msg << myPort_ << "(" << x_pos << ", " << y_pos << ")";

	TalkToAll(msg.str(), defautBroadCastPort);
}

bool Follower::ParseMsg(const unsigned char * msg, size_t length)
{
	string strMsg(msg, msg + length);
	if(strMsg.compare(stopMsg) == 0)
	{
		SendLocation();
		return false;
	}

	return true;
}

void Follower::handle_read(unsigned char * buf, const boost::system::error_code& error, size_t bytes_transferred)
{
	if(!error && bytes_transferred < 0)
	{
		cout <<"Receive msg: "<< string(buf, buf + bytes_transferred) << endl;
		if(ParseMsg(buf, bytes_transferred))
		{
			cout << "follower is running..." << endl;
			Walk();
		}
		else
		{
			Stop();
		}
	}

	ListenFromAll();
}

void Follower::Run()
{
   	ListenFromAll(boost::bind(&Follower::handle_read, this, _1, _2, _3));
  	TalkToAll("Test Send", defautBroadCastPort);
  	SendLocation();
}