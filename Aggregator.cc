#include "Aggregator.h"
#include <math.h>

using namespace std;

const double forwardSpeed = 0.5;

Aggregator::Aggregator(boost::asio::io_service & io_service, string host, int player_port)
:Centralization(io_service, host, player_port)
{
	cout << "Create a Aggregator" << endl;
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

	return location.getDistance(center) > (getInterDistance() * 0.1);
}

void Aggregator::Moving(CoorPtr destination)
{
	if (ComapreToCenter(destination))
	{
		double newspeed = 0.0;
		double avoid_yaw = 0.0;
		LaserAvoidance(newspeed, avoid_yaw);
		//SetSpeed(newspeed, avoid_yaw);

		double diffY = destination->getY() - GetYPos();
		double diffX = destination->getX() - GetXPos();
		double desired_yaw = atan2(diffY, diffX) - GetYaw();

		SetSpeed(newspeed, (desired_yaw * 3 + avoid_yaw) / 4);

		//GoTo(destination->getX(), destination->getY());
	}
	else
	{
		Stop();
	}
}