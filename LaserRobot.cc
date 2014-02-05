#include "LaserRobot.h"
#include "Communicate.h"

const int defaultPosition2dProxyPort = 0;
const int defaultLaserProxyPort = 0;

using namespace std;


LaserRobot::LaserRobot(string host, int port)
:myHost(host),
myPort(port)
{
	robot = new PlayerClient(host, port);
	pp = new Position2dProxy(robot, defaultPosition2dProxyPort);
	lp = new LaserProxy(robot, defaultLaserProxyPort);
}

LaserRobot::~LaserRobot()
{
	delete lp;
	delete pp;
	delete robot;
}

int LaserRobot::StartMoving()
{
  pp->SetSpeed(5.0, 5.0, 5.0);
}

int LaserRobot::StopMoving()
{
  pp->SetSpeed(0.0, 0.0, 0.0);

  return 0;
}

int LaserRobot::LaserAvoidance()
{
      double newspeed = 0;
      double newturnrate = 0;

      // this blocks until new data comes; 10Hz by default
      robot->Read();

      double minR = lp->GetMinRight();
      double minL = lp->GetMinLeft();

      // laser avoid (stolen from esben's java example)
      //std::cout << "minR: " << minR
      //          << "minL: " << minL
      //          << std::endl;

      double l = (1e5*minR)/500-100;
      double r = (1e5*minL)/500-100;

      if (l > 100)
        l = 100;
      if (r > 100)
        r = 100;

      newspeed = (r+l)/1e3;

      newturnrate = (r-l);
      newturnrate = limit(newturnrate, -40.0, 40.0);
      newturnrate = dtor(newturnrate);

      //std::cout << "speed: " << newspeed
      //          << "turn: " << newturnrate
      //          << std::endl;

      // write commands to robot
      pp->SetSpeed(newspeed, newturnrate);
}

int LaserRobot::CreateListen(int port, int index)
{
  return create_listen(port, index);
}

int LaserRobot::CreateBroadcast(int port, int index)
{
  return create_broadcast(port, index);
}

int LaserRobot::ListenFromAll(int id, char * msg)
{
  return listen_to_robot(id, msg);
}

int LaserRobot::TalkToAll(int id, int index, char * msg)
{
  return talk_to_all(id, msg, index);
}

void LaserRobot::Run()
{
  cout << "LaserRobot is running" << endl;

	for(;;)
  {
    LaserAvoidance();
  }
}