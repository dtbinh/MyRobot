#include <iostream>
#include <stdlib.h>
#include <libplayerc++/playerc++.h>
#include <boost/lexical_cast.hpp>
#include "RobotFactory.h"

using namespace std;

const int defaultPort = 6665;
const string defaultHost = "localhost";

int GetOneInput(string & val, string name)
{
	using namespace boost;

	cout<<"input "<<name<<": "<<endl;
	cin >> val;	

	if (val.compare("a") == 0)
	{
		return robotAggregator;
	}
	else if(val.compare("b") == 0)
	{
		return robotDispersor;
	}

	cerr << "cannot parse parameter from input : unknown run_type" << endl;
	return GetOneInput(val, name);
}

int GetOneInput(int & val, string name)
{
	using namespace boost;

	cout<<"input "<<name<<": "<<endl;

	string input;
	cin >> input;
	try
	{
		val = lexical_cast<int>(input);
	}
	catch (const bad_lexical_cast & e)
	{
		cerr << "cannot parse parameter from input : " << e.what() << endl;
		return GetOneInput(val, name);
	}

	return val;
}

bool GetInput(int & player_port, string & run_type, int & d_sense, int & distance )
{
	using namespace boost;

	cout << "input error, you have to input parameters one by one :( "<<endl;

	GetOneInput(player_port, "player_port");

	int robot_type = GetOneInput(run_type, "run_type");

	GetOneInput(d_sense, "d_sense");

	GetOneInput(distance, "distance");
	if (distance >= d_sense)
	{
		cerr << "required distance < d_sense, but "<<distance<<" >= "<<d_sense<< endl;
		GetInput(player_port, run_type, d_sense, distance);
	}

	return true;
}

int main(int argc, char *argv[])
{
	try
	{
		int player_port = 6665;
		string run_type = "a";
		int d_sense = 140;
		int distance = 100;

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
		else if (run_type.compare("b") == 0)
		{
			robot_type = robotDispersor;
		}
		else 
		{
			robot_type = robotFollower;
		}

		boost::scoped_ptr<Robot> robot(RobotFactory::CreateRobot(robot_type, io_service_, defaultHost, player_port));
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

