/**
 * formationSize is not used in this version becasue of laziness... kern 03/13/2014
 */
#include "Line.h"
#include <boost/make_shared.hpp>

 using namespace boost;

Line::Line(double interval, size_t formationSize)
:Formation(interval, formationSize)
{

}

Line::~Line()
{

}

CoorPtr Line::CalcVerticeToLeader(size_t index, CoorPtr source)
{
	double x = source->getX();
	double y = source->getY();

	switch(index)
	{
		case 1:
			y += interval_;
			break;

		case 2:
			y -= interval_;
			break;

		case 3:
			y -= interval_ * 2;
			break;
	}

	return make_shared<Coordinate>(x,y);
}

double Line::CalcIntervalToLeader(size_t index)
{	
	double interval = 0;

	switch(index)
	{
		case 1:
			interval = interval_;
			break;

		case 2:
			interval = interval_;
			break;

		case 3:
			interval = interval_ * 2;
			break;

		default:
			break;
	}

	return interval;
}