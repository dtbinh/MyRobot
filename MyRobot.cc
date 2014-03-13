#include <iostream>
#include <stdlib.h>
#include <libplayerc++/playerc++.h>
#include <boost/lexical_cast.hpp>
#include "RobotFactory.h"
#include "Centralization.h"

using namespace std;

const int defaultPort = 6665;
const string defaultHost = "localhost";
const double inch2meter = 0.0254;

int main(int argc, char *argv[])
{
	try
	{
		int player_port;
		string run_type;
		int d_sense;
		int distance;

		if (argc == 5)
		{
			using namespace boost;

			try
			{
				player_port = lexical_cast<int>(argv[1]);
				run_type = argv[2];
				d_sense = lexical_cast<int>(argv[3]);
				distance = lexical_cast<int>(argv[4]);

				if (distance >= d_sense)
				{
					cerr << "required distance < d_sense, but "<<distance<<" >= "<<d_sense<< endl;
					GetInput(player_port, run_type, d_sense, distance);
				}

			}
			catch (const bad_lexical_cast & e)
			{
				cerr << "cannot parse parameters from input : " << e.what() << endl;
				GetInput(player_port, run_type, d_sense, distance);
			}
		}
		else
		{
			GetInput(player_port, run_type, d_sense, distance);
		}

		boost::asio::io_service io_service_;
		boost::asio::io_service::work work_(io_service_);

		int robot_type = -1;
		if (run_type.compare("a") == 0)
		{
			robot_type = robotAggregator;
		}
		else if (run_type.compare("d") == 0)
		{
			robot_type = robotDispersor;
		}
		else if (run_type.compare("l") == 0)
		{
			robot_type = robotLeader;
		}
		else if (run_type.compare("f") == 0)
		{
			robot_type = robotFollower;
		}

		boost::shared_ptr<Robot> robot(RobotFactory::CreateRobot(robot_type, io_service_, defaultHost, player_port));
		boost::static_pointer_cast<Centralization>(robot)->setDsense(d_sense * inch2meter);
		boost::static_pointer_cast<Centralization>(robot)->setInterDistance(distance * inch2meter);

		robot->Run();
		io_service_.run();

		cout << "Robot Exit" << endl;
	}
	catch (PlayerCc::PlayerError & e)
	{
		cerr << "main catch player exception: " << e << endl;
		return -1;
	}
	catch (std::exception & e)
	{
		cerr << "main catch std exception: " << e.what() << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "main catch unknown exception" << endl;
		return -1;
	}
}

