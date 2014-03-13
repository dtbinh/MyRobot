#include <iostream>
#include <stdlib.h>
#include <libplayerc++/playerc++.h>
#include <boost/lexical_cast.hpp>
#include "RobotFactory.h"

using namespace std;

const string defaultHost = "localhost";
//const double inch2meter = 0.0254;

int main(int argc, char *argv[])
{
    try
  	{
    	string strHost = defaultHost;
  		int player_port = defaultPort;

  		if (argc >= 3)
  		{
  	  		strHost = argv[1];
  	  		player_port = atoi(argv[2]);
  		}
  		else if (argc >= 2)
  		{
  	  		player_port = atoi(argv[1]);
  		}

		boost::asio::io_service io_service_;
		boost::asio::io_service::work work_(io_service_);

		int robot_type = -1;
		if (player_port == defaultPort)
		{
			robot_type = robotFormationLeader;
		}
		else
		{
			robot_type = robotFormationFollower;
		}

		boost::shared_ptr<Robot> robot(RobotFactory::CreateRobot(robot_type, io_service_, defaultHost, player_port));

		robot->Run();
		io_service_.run();

		cout << "Robot Exit" << endl;
    
    }
    catch (PlayerCc::PlayerError & e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
}  

