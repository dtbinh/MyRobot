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

bool GetInput(int & player_port, string & run_type, int & d_sense, int & distance, int & d_agorithm )
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
		GetInput(player_port, run_type, d_sense, distance, d_agorithm);
	}

	if (robot_type == robotAggregator)
	{
		GetOneInput(d_agorithm, "d_aggreagte");
	}
	else if(robot_type == robotDispersor)
	{
		GetOneInput(d_agorithm, "d_disperse");
	}
	else
	{
		cerr << "cannot parse parameter from input : unknown run_type" << endl;
		GetInput(player_port, run_type, d_sense, distance, d_agorithm);	
	}
	
	return true;
}

int main(int argc, char *argv[])
{
	try
	{
		int player_port = 0;
		string run_type = "";
		int d_sense = 0;
		int distance = 0;
		int d_agorithm = 0;

		if (argc == 6)
		{
			using namespace boost;

			try
			{
				player_port = lexical_cast<int>(argv[1]);
				run_type = argv[2];
				d_sense = lexical_cast<int>(argv[3]);
				distance = lexical_cast<int>(argv[4]);
				d_agorithm = lexical_cast<int>(argv[5]);

				if (distance >= d_sense)
				{
					cerr << "required distance < d_sense, but "<<distance<<" >= "<<d_sense<< endl;
					GetInput(player_port, run_type, d_sense, distance, d_agorithm);
				}

			}
			catch (const bad_lexical_cast & e)
			{
				cerr << "cannot parse parameters from input : " << e.what() << endl;
				GetInput(player_port, run_type, d_sense, distance, d_agorithm);
			}
		}
		else
		{
			GetInput(player_port, run_type, d_sense, distance, d_agorithm);
		}

		boost::asio::io_service io_service_;
		boost::asio::io_service::work work_(io_service_);

		int robot_type = -1;
		if (run_type.compare("a") == 0)
		{
			robot_type = robotAggregator;
			cout << "Create a Aggregator" << endl;
		}
		else
		{
			robot_type = robotDispersor;
			cout << "Create a Dispersor" << endl;
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

