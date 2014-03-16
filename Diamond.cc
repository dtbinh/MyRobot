/**
 * formationSize is not used in this version becasue of laziness... kern 03/13/2014
 */
#include "Diamond.h"
#include <boost/make_shared.hpp>
#include <math.h>

using namespace boost;

Diamond::Diamond(double interval, size_t formationSize)
:Formation(interval, formationSize)
{

}

Diamond::~Diamond()
{

}

CoorPtr Diamond::CalcVerticeToLeader(size_t index, CoorPtr source)
{
	double x = source->getX();
	double y = source->getY();

	switch(index)
	{
		case 1:
			x -= interval_;
			y += interval_;
			break;

		case 2:
			x -= interval_;
			y -= interval_;
			break;

		case 3:
			x -= interval_ * 2;
			break;

		default:
			break;
	}

	return make_shared<Coordinate>(x,y);
}

double Diamond::CalcIntervalToLeader(size_t index)
{
	double interval = 0;
	switch(index)
	{
		case 1:
			interval = sqrt(2) * interval_;
			break;

		case 2:
			interval = sqrt(2) * interval_;
			break;

		case 3:
			interval = interval_ * 2;
			break;

		default:
			break;
	}

	return interval;
}