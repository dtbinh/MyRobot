#include "Dispersor.h"

using namespace std;

Dispersor::Dispersor(boost::asio::io_service & io_service, string host, int player_port)
:Centralization(io_service, host, player_port)
{
	cout << "Create a Dispersor" << endl;
}

Dispersor::~Dispersor()
{

}

void Dispersor::Identify()
{
	cout << "Dispersor is running" << endl;
}

bool Dispersor::CompareToInterRobot(CoorPtr other)
{
	Coordinate location(GetXPos(), GetYPos());

	return location.getDistance(other) < getInterDistance();
}

void Dispersor::Moving(CoorPtr source)
{
	double diffY = GetYPos() - source->getY();
	double diffX = GetXPos() - source->getX();

	double desired_yaw = atan2(diffY, diffX);
	double current_yaw = GetYaw();

	double diff_yaw = desired_yaw - current_yaw;

	SetSpeed(forwardSpeed, diff_yaw);
}