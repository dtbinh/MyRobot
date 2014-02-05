#include "Leader.h"
#include "TimeRecorder.h"

using namespace std;

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9001;
const int listenChannel = 0;
const int broadcastChannel = 0; 

const int duration = 20;
const int follower_num = 5;

Leader::Leader(string host, int port)
:LaserRobot(host, port)
{
	listenID = CreateListen(defaultListenPort, listenChannel);
	broadcastID = CreateBroadcast(defautBroadCastPort, broadcastChannel);
}

Leader::~Leader()
{
	
}

void Leader::Listen(int countCatchMsgNum)
{
	sleep(1);

	int recvBytes = ListenFromAll(listenID, msg);

	if(recvBytes > 0)
	{
		cout <<"Recive msg: "<< msg << endl;	
	}

	countCatchMsgNum += recvBytes / locationMsgLen;

	if(countCatchMsgNum >= follower_num)
	{
		Resume();
	}
	else 
	{
		Listen(countCatchMsgNum);
	}
}

void Leader::Walk(TimeRecorder & tr, bool bFirst)
{
	double dur = tr.getPeriod();
	//cout <<"dur = " << dur << endl;
	
	if (dur >= duration)
	{
		string msg = stopMsg;
		char *cstr = new char[msg.length() + 1];
		strcpy(cstr, msg.c_str());
		TalkToAll(broadcastID, broadcastChannel, cstr);
		cout<<"Send msg: " << msg << endl;

		if (bFirst)
		{
			cout << duration << " seconds passed. Current's robot locations are:" << endl;
			StopMoving();
			Listen(0);
		}		
		else
		{
			StopMoving();
			cout << duration << " seconds passed. Mission accomplished." << endl;
		}

		delete cstr;
	}
	else
	{
		LaserAvoidance();	
		Walk(tr, bFirst);
	}
}

void Leader::Resume()
{
	string msg = resumeMsg;
	char *cstr = new char[msg.length() + 1];
	strcpy(cstr, msg.c_str());

	TalkToAll(broadcastID, broadcastChannel, cstr);
	cout<<"Send msg: " << msg << endl;

	cout << "All robots resume safewalking." << endl;

	StartMoving();
    TimeRecorder tr;
	Walk(tr, false);
}

void Leader::Start()
{
	string msg = startMsg;
	char *cstr = new char[msg.length() + 1];
	strcpy(cstr, msg.c_str());

	TalkToAll(broadcastID, broadcastChannel, cstr);
	cout<<"Send msg: " << msg << endl;

	cout<<"All robots start safewalking."<<endl;

	StartMoving();
    TimeRecorder tr;
	Walk(tr, true);
}

void Leader::Run()
{
	cout << "waiting for input s" << endl;
	char cmd;
	cin >> cmd;

	switch(cmd)
	{
		case 's':
			Start();
			break;

		case 'S':
			Start();
			break;

		default:
			Run();
			break;
	}
}