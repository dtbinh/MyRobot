#include "RobotFactory.h"
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