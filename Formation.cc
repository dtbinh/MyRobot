#include "Formation.h"
#include <boost/math/constants/constants.hpp>

using namespace boost;

Formation::Formation(double interval, size_t formationSize)
:interval_(interval),
formationSize_(formationSize)
{

}

Formation::~Formation()
{

}

bool Formation::CalcSpeed(size_t index, double curYaw, double leaderSpeed, double leaderYaw, CoorPtr leader, CoorPtr self, double & outSpeed, double & outYaw)
{
    CoorPtr destination = CalcVerticeToLeader(index, leader);

	double distanceDesired2leader = CalcIntervalToLeader(index);
	double distance2leader = leader->getDistance(self);	
    double distance2destination = destination->getDistance(self);
	outSpeed = leaderSpeed + (distance2destination / distanceDesired2leader) * leaderSpeed; 

	double diffY = destination->getY() - self->getY();
    double diffX = destination->getX() - self->getX();   
    double min_yaw = math::constants::pi<double>() / -2;
    double max_yaw = math::constants::pi<double>() / 2;
    double desiedYaw = atan2(diffY, diffX);
    if (desiedYaw < min_yaw)
    {
        desiedYaw = min_yaw;
        outSpeed = 0.2;
    }
    else if (desiedYaw > max_yaw)
    {
        desiedYaw = max_yaw;
        outSpeed = 0.2;
    }

    outYaw = (desiedYaw - curYaw);
    if (distance2destination < interval_)
    {
        outYaw *= distance2destination / interval_;
    }

    return distance2destination < DistacneThreshold;
}