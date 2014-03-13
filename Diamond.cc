/**
 * formationSize is not used in this version becasue of laziness... kern 03/13/2014
 */
#include "Diamond.h"

using namespace boost;

Diamond::Diamond(double interval, size_t formationSize)
:Formation(interval, formationSize)
{

}

Diamond::~Diamond()
{

}

CoorPtr Diamond::CalcVertice(size_t index, CoorPtr source)
{
	double x = source->getX();
	double y = source->getY();

	switch(index)
	{
		case 1:
			x -= interval;
			y -= interval;
			break;

		case 3:
			x -= interval;
			y += interval;
			break

		case 4:
			x -= interval * 2;
			break;

		default:
			break;
	}

	return make_shared<Coordinate>(x,y);
}