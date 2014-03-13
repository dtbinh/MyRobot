#pragma once
#include <boost/shared_ptr.hpp>
#include "Coordinate.h"

class Formation
{
public:
	Formation(double interval, size_t formationSize);
	~Formation();

	virtual CoorPtr CalcVertice(size_t index, CoorPtr source) = 0;

protected:
	double interval_;
	size_t formationSize_;	
};

typedef boost::shared_ptr<Formation> FormationPtr;