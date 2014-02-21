#include "Follower.h"

using namespace std;

const int defaultListenPort = 9001;
const int defautBroadCastPort = 9000;
const int listenChannel = 0;
const int broadcastChannel = 0; 

Follower::Follower(boost::asio::io_service & io_service, string host, int player_port)
:myPort_(player_port),
LaserRobot(io_service, defaultListenPort, host, player_port)
{

}

Follower::~Follower()
{
	
}

void Follower::SendLocation()
{
	int x_pos = GetXPos();
	int y_pos = GetYPos();

	ostringstream msg;
	msg << myPort_ << "(" << x_pos << ", " << y_pos << ")" << endl;

	TalkToAll(msg.str(), defautBroadCastPort);

	cout<<"Send msg: " << msg.str() << endl;
}

bool Follower::ParseMsg(const char * msg)
{
	cout <<"Receive msg: "<< msg << endl;

	if(strcmp(msg, stopMsg.c_str()) == 0)
	{
		SendLocation();
		return false;
	}

	return true;
}

int Follower::handle_read(const unsigned char * buf, size_t bytes_transferred)
{
	ListenFromAll(boost::bind(&Follower::handle_read, this, _1, _2));

	if(bytes_transferred > 0)
	{	
		if(ParseMsg((const char *)buf))
		{
			cout << "follower is running..." << endl;
			Walk();
		}
		else
		{
			Stop();
		}
	}
	
	return bytes_transferred;
}

void Follower::Run()
{
	ListenFromAll(boost::bind(&Follower::handle_read, this, _1, _2));
}