#include "Follower.h"

using namespace std;

const int defaultListenPort = 9001;
const int defautBroadCastPort = 9000;
const int listenChannel = 0;
const int broadcastChannel = 0; 

Follower::Follower(string host, int port)
:LaserRobot(host, port)
{
	listenID = CreateListen(defaultListenPort, listenChannel);
	broadcastID = CreateBroadcast(defautBroadCastPort, broadcastChannel);
}

Follower::~Follower()
{
	
}

int Follower::SendLocation()
{
	ostringstream msg;

	int x_pos = pp->GetXPos();
	int y_pos = pp->GetYPos();

	msg << myPort << "(" << x_pos << ", " << y_pos << ")" << endl;

	char *cstr = new char[msg.str().length() + 1];
	strcpy(cstr, msg.str().c_str());

	TalkToAll(broadcastID, broadcastChannel, cstr);

	cout<<"Send msg: " << msg.str() << endl;

	delete cstr;
	
	return 0;
}

bool Follower::ParseMsg(char * msg)
{
	cout <<"Receive msg: "<< msg << endl;

	bool ret = false;

	if (strcmp(msg, startMsg.c_str()) == 0)
	{
		ret = true;
		StartMoving();
	}
	else if(strcmp(msg, resumeMsg.c_str()) == 0)
	{
		ret = true;
		StartMoving();
	}
	else if(strcmp(msg, stopMsg.c_str()) == 0)
	{
		StopMoving();
		SendLocation();
	}

	return ret;
}

void Follower::Run()
{
	cout << "follower is running..." << endl;

	bool bWalk = false;

	for(;;)
	{
		int recvBytes = ListenFromAll(listenID, msg);
		if(recvBytes > 0)
		{
			bWalk = ParseMsg(msg);
		}

		if (bWalk)
		{
			LaserAvoidance();	
		}
		else
		{
			sleep(1);
		}
		
	}
}