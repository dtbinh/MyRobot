#pragma once
#include <boost/shared_ptr.hpp>
#include "Coordinate.h"

const double robot_interval = 1.0;
const size_t robot_size = 4;
const double DistacneThreshold = 0.1;

class Formation
{
public:
	Formation(double interval, size_t formationSize);
	~Formation();

	bool CalcSpeed(size_t index, double curYaw, double leaderSpeed, double leaderYaw, CoorPtr leader, CoorPtr self, double & outSpeed, double & outYaw);

	virtual CoorPtr CalcVerticeToLeader(size_t index, CoorPtr source) = 0;

protected:
	virtual double CalcIntervalToLeader(size_t index) = 0;

protected:
	double interval_;
	size_t formationSize_;	
};

typedef boost::shared_ptr<Formation> FormationPtr;