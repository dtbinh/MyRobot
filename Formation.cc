#include "Formation.h"
#include <boost/math/constants/constants.hpp>

using namespace boost;

const double DistacneThreshold = 0.05;

Formation::Formation(double interval, size_t formationSize)
:interval_(interval),
formationSize_(formationSize)
{

}

Formation::~Formation()
{

}

bool Formation::CalcSpeed(size_t index, double leaderSpeed, double leaderYaw, CoorPtr leader, CoorPtr self, double & outSpeed, double & outYaw)
{
	double interval = CalcIntervalToLeader(index);
	double distance2leader = leader->getDistance(self);	
	outSpeed = pow((distance2leader / interval),2) * leaderSpeed;

	CoorPtr destination = CalcVerticeToLeader(index, leader);
	double diffY = destination->getY() - self->getY();
    double diffX = destination->getX() - self->getX();   
    double min_yaw = math::constants::pi<double>() / -2;
    double max_yaw = math::constants::pi<double>() / 2;
    outYaw = atan2(diffY, diffX);
    if (outYaw < min_yaw)
    {
        outYaw = min_yaw;
        outSpeed = 0.0;
    }
    else if (outYaw > max_yaw)
    {
        outYaw = max_yaw;
        outSpeed = 0.0;
    }

    return destination->getDistance(self) < DistacneThreshold;
}