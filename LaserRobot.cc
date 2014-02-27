#include "LaserRobot.h"

const int defaultPosition2dProxyPort = 0;
const int defaultLaserProxyPort = 0;

using namespace std;

LaserRobot::LaserRobot(boost::asio::io_service & io_service, string host, int player_port)
:timerWalk_(io_service)
{
  robot_ = new PlayerClient(host, player_port);
	pp_ = new Position2dProxy(robot_, defaultPosition2dProxyPort);
	lp_ = new LaserProxy(robot_, defaultLaserProxyPort);

  pp_->RequestGeom();
}

LaserRobot::~LaserRobot()
{
  cout<<"~LaserRobot()"<<endl;
  
	delete lp_;
	delete pp_;
	delete robot_;
}

double LaserRobot::GetXPos()
{
  return pp_->GetXPos();
}

double LaserRobot::GetYPos()
{
  return pp_->GetYPos();
}

double LaserRobot::GetYaw()
{
  return pp_->GetYaw();
}

int LaserRobot::StartMoving()
{
  pp_->SetSpeed(1.0, 0.0);
}

int LaserRobot::StopMoving()
{
  pp_->SetSpeed(0.0, 0.0);

  return 0;
}

void LaserRobot::SetSpeed(double forwardSpeed, double turnSpeed)
{
  pp_->SetSpeed(forwardSpeed, turnSpeed);
}

void LaserRobot::LaserAvoidance()
{
  double newspeed = 0;
  double newturnrate = 0;

  // this blocks until new data comes; 10Hz by default
  robot_->Read();

  double minR = lp_->GetMinRight();
  double minL = lp_->GetMinLeft();

  // laser avoid (stolen from esben's java example)
  std::cout << "minR: " << minR
            << "minL: " << minL
            << std::endl;

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

  std::cout << "speed: " << newspeed
            << " turn: " << newturnrate
            << std::endl;

  // write commands to robot
  pp_->SetSpeed(newspeed, newturnrate);
}

void LaserRobot::handle_timerWalk(const boost::system::error_code& error)
{
  if (!error)
  {
    Resume();
  }
}

void LaserRobot::Resume()
{
    LaserAvoidance();
    timerWalk_.expires_from_now(boost::posix_time::millisec(200));
    timerWalk_.async_wait(boost::bind(&LaserRobot::handle_timerWalk, this, boost::asio::placeholders::error));
}

void LaserRobot::Walk()
{
  StartMoving();
  Resume();
}

void LaserRobot::Stop()
{
  timerWalk_.cancel();
  StopMoving();
}

void LaserRobot::Run()
{
  cout << "LaserRobot is running" << endl;

  Walk();
}