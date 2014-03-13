/**
 * formationSize is not used in this version becasue of laziness... kern 03/13/2014
 */
#include "Line.h"

 using namespace boost;

Line::Line(double interval, size_t formationSize)
:Formation(interval, formationSize)
{

}

Line::~Line()
{

}

CoorPtr Line::CalcVertice(size_t index, CoorPtr source)
{
	double x = source->getX();
	double y = source->getY() + (index - 2) * interval;

	return make_shared<Coordinate>(x,y);
}