#include "Leader.h"

using namespace std;

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9001;

const int duration = 20;
const int follower_num = 5;

Leader::Leader(boost::asio::io_service & io_service, string host, int player_port)
:LaserRobot(io_service, host, player_port),
CommPoint(io_service, defaultListenPort),
timerCount_(io_service)
{
	cout << "Create a Leader" << endl;
	countMsg_ = 0;
}

Leader::~Leader()
{
	
}

void Leader::handle_read(unsigned char * buf, const boost::system::error_code& error, size_t bytes_transferred)
{
	if(!error && bytes_transferred > 0)
	{
		cout <<"Recive msg: "<< string(buf, buf+bytes_transferred) << endl;	

		countMsg_ += bytes_transferred / locationMsgLen;
				
		if(countMsg_ >= follower_num)
		{
			countMsg_ = 0;
			Start(false);
		}
		else
		{
			ListenFromAll(boost::bind(&Leader::handle_read, this, _1, _2, _3));	
		}
	}
}

void Leader::handle_timerCount(const boost::system::error_code& error, bool bFirstCount)
{
	if (!error)
  	{
  		TalkToAll(stopMsg, defautBroadCastPort);

    	if (bFirstCount)
    	{
    		cout << duration << " seconds passed. Current's robot locations are:" << endl;
    		Stop();
    		ListenFromAll(boost::bind(&Leader::handle_read, this, _1, _2, _3));	
    	}
    	else
    	{
    		cout << duration << " seconds passed. Mission accomplished." << endl;
    		Stop();
    	}
  	}
}

void Leader::Start(bool bFirstTime)
{
	string msg = "";
	if (bFirstTime)
	{
		msg = startMsg;
	}
	else
	{
		msg = resumeMsg;
	}

	TalkToAll(msg, defautBroadCastPort);

	cout<<"All robots "<<msg<<" safewalking."<<endl;

	timerCount_.expires_from_now(boost::posix_time::seconds(duration));
  	timerCount_.async_wait(boost::bind(&Leader::handle_timerCount, this, boost::asio::placeholders::error, bFirstTime));

	cout << "leader is running..." << endl;
  	Walk();
}

void Leader::Run()
{
	cout << "waiting for input s" << endl;
	char cmd;
	cin >> cmd;

	switch(cmd)
	{
		case 's':
			Start(true);
			break;

		case 'S':
			Start(true);
			break;

		default:
			Run();
			break;
	}
}