#include "Leader.h"
#include "TimeRecorder.h"

using namespace std;

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9001;

const int duration = 20;
const int follower_num = 5;

Leader::Leader(boost::asio::io_service & io_service, string host, int player_port)
:LaserRobot(io_service, defaultListenPort, host, player_port),
timerCount_(io_service)
{
	countMsg_ = 0;
}

Leader::~Leader()
{
	
}

int Leader::handle_read(const unsigned char * buf, size_t bytes_transferred)
{
	if(bytes_transferred > 0)
	{
		cout <<"Recive msg: "<< buf << endl;	

		countMsg_ += bytes_transferred / locationMsgLen;
				
		if(countMsg_ >= follower_num)
		{
			countMsg_ = 0;
			Start(false);
		}
		else
		{
			ListenFromAll(boost::bind(&Leader::handle_read, this, _1, _2));	
		}
	}
	
	return bytes_transferred;
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
    		ListenFromAll(boost::bind(&Leader::handle_read, this, _1, _2));	
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
	cout<<"Send msg: " << msg << endl;

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