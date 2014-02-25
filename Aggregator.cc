#include "Aggregator.h"
#include <math.h>

using namespace std;

Aggregator::Aggregator(boost::asio::io_service & io_service, string host, int player_port)
:Centralization(io_service, host, player_port)
{

}

Aggregator::~Aggregator()
{

}

void Aggregator::Identify()
{
	cout << "Aggregator is running" << endl;
}

bool Aggregator::CompareToInterRobot(CoorPtr other)
{
	Coordinate location(GetXPos(), GetYPos());

	return location.getDistance(other) > getInterDistance();
}

bool Aggregator::ComapreToCenter(CoorPtr center)
{
	Coordinate location(GetXPos(), GetYPos());

	return location.getDistance(center) > getDagorithm();
}

void Aggregator::Moving(CoorPtr destination)
{
	double diffY = destination->getY() - GetYPos();
	double diffX = destination->getX() - GetXPos();

	double desired_yaw = atan2(diffY, diffX);
	double current_yaw = GetYaw();	

	double diff_yaw = desired_yaw - current_yaw;

	SetSpeed(forwardSpeed, diff_yaw);
}