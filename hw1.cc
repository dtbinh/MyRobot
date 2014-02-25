#include <iostream>
#include <stdlib.h>

#include "RobotFactory.h"
#include "LaserRobot.h"

using namespace std;

const int defaultPort = 6665;
const string defaultHost = "localhost";

int main(int argc, char *argv[])
{
  try
  {
    string strHost = defaultHost;
  	int iPort = defaultPort;

  	if (argc >= 3)
  	{
  	  strHost = argv[1];
  	  iPort = atoi(argv[2]);
  	}
  	else if (argc >= 2)
  	{
  	  iPort = atoi(argv[1]);
  	}

    boost::asio::io_service io_service_;
    boost::asio::io_service::work work_(io_service_);

    boost::shared_ptr<Robot> robot;
    if (iPort == defaultPort)
    {
      cout << "Create a leader" << endl;
      robot.reset(RobotFactory::CreateRobot(robotLeader, io_service_, strHost, iPort));
    }
    else
    {
      cout << "Create a follower" << endl;
      robot.reset(RobotFactory::CreateRobot(robotFollower, io_service_, strHost, iPort));
    }

    robot->Run();
    io_service_.run();
    
    cout << "Robot Exit" << endl;
    
  }
  catch (PlayerCc::PlayerError & e)
  {
    cerr << "main catch player exception: " << e << endl;   
    return -1;
  }
  catch (exception & e)
  {
    cerr << "main catch std exception: " << e.what() << endl;  
    return -1;
  }
  catch (...)
  {
    cerr<< "main catch unknown exception"<<endl;
    return -1;
  }
}

