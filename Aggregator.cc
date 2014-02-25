#include "Aggregator.h"
#include <math.h>

using namespace std;

const double forwardSpeed = 2;

Aggregator::Aggregator(boost::asio::io_service & io_service, string host, int player_port)
:LaserRobot(io_service, host, player_port),
Centralization(io_service, player_port),
timerWalk_(io_service)
{

}

Aggregator::~Aggregator()
{

}

int Aggregator::getDsense()
{
	return d_sense_;
}

void Aggregator::setDsense(int val)
{
	d_sense_ = val;
}

int Aggregator::getDaggregate()
{
	return d_aggregate_;
}

void Aggregator::setDaggregate(int val)
{
	d_aggregate_ = val;
}

void Aggregator::setInterDistance(int val)
{
	inter_distance_ = val;
}

int Aggregator::getInterDistance()
{
	return inter_distance_;
}

bool Aggregator::compare(double distance)
{
	return distance > getInterDistance();
}

void Aggregator::MoveTowards(CoorPtr destination)
{
	double current_yaw = GetYaw();
	
	double diffY = destination->getY() - location_.getY();
	double diffX = destination->getX() - location_.getX();

	double desired_yaw = PlayerCc::dtor(atan(diffY / diffX));
}

void Aggregator::Resume()
{
	LaserAvoidance();
	BroadcastLocation(GetXPos(), GetYPos());
	
	CoorPtr center;
	if(CheckCenter(FilterNeighbor(getDsense()), center, boost::bind(&Aggregator::compare, this, _1)) 
		&& location_.getDistance(center) >= getDaggregate())
	{
		MoveTowards(center);
	}
	else
	{
		Stop();
	}

	timerWalk_.expires_from_now(boost::posix_time::millisec(200));
    timerWalk_.async_wait(boost::bind(&Aggregator::handle_timerWalk, this, boost::asio::placeholders::error));
}

void Aggregator::handle_timerWalk(const boost::system::error_code& error)
{
	if (!error)
 	{
 		Resume();
 	}
}

void Aggregator::Run()
{
	RegisterListening();

	Resume();
}