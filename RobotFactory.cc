#include "RobotFactory.h"
#include "Leader.h"
#include "Follower.h"
#include "Aggregator.h"
#include "Dispersor.h"
#include "FormationLeader.h"
#include "FormationFollower.h"

using namespace std;

RobotFactory::RobotFactory()
{

}

RobotFactory::~RobotFactory()
{

}

Robot * RobotFactory::CreateRobot(int type, boost::asio::io_service & io_service, string host, int player_port)
{
	Robot * ret = NULL;

	switch(type)
	{
		case robotLeader:
			ret = new Leader(io_service, host, player_port);
			break;

		case robotFollower:
			ret = new Follower(io_service, host, player_port);
			break;

		case robotAggregator:
			ret = new Aggregator(io_service, host, player_port);
			break;

		case robotDispersor:
			ret = new Dispersor(io_service, host, player_port);
			break;

		case robotFormationLeader:
			ret = new FormationLeader(io_service, host, player_port);
			break;

		case robotFormationFollower:
			ret = new FormationFollower(io_service, host, player_port);
			break;
			
		default:
			break;
	}

	return ret;
}