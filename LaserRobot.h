#pragma once

#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

const std::string startMsg = "start";
const std::string stopMsg = "stop";
const std::string resumeMsg = "resume";

const int max_msg_len = 256;
const int locationMsgLen = 11;

class LaserRobot
{
private:
	PlayerClient * robot;
	LaserProxy * lp;

protected:
	std::string myHost;
	int myPort;
	Position2dProxy * pp;

protected:
	int listenID;
	int broadcastID;

protected:
	int LaserAvoidance();
	int StopMoving();
	int StartMoving();

	int CreateListen(int port, int index);
	int CreateBroadcast(int port, int index);
	int ListenFromAll(int id, char * msg);
	int TalkToAll(int id, int index, char * msg);

public:
	LaserRobot(std::string host, int port);
	~LaserRobot();

	virtual void Run();
};