#include "Coordinate.h"
#include <boost/make_shared.hpp>
#include <math.h>

using namespace std;

Coordinate::Coordinate(double x, double y)
:x_(x),
y_(y)
{
	
}

Coordinate::~Coordinate()
{

}

double Coordinate::getX()
{
	return x_;
}

double Coordinate::getY()
{
	return y_;
}

void Coordinate::setX(double x)
{
	x_ = x;
}

void Coordinate::setY(double y)
{
	y_ = y;
}

double Coordinate::getDistance(double x_pos, double y_pos)
{
	return sqrt(pow((x_pos - getX()), 2) + pow((y_pos - getY()), 2));
}

double Coordinate::getDistance(CoorPtr other)
{
	return getDistance(other->getX(), other->getY());
}

CoorPtr Coordinate::CalCenter(queue<CoorPtr> others)
{
	if (others.size() == 0)
	{
		return shared_from_this();
	}
	else if(others.size() == 1)
	{
		return CalCenter(others.front());
	}

	CoorPtr left = others.front();
	others.pop();
	CoorPtr right = others.front();
	others.pop();

	others.push(CalCenter(left, right));
	
	return CalCenter(others);
}

CoorPtr Coordinate::CalCenter(CoorPtr l, CoorPtr r)
{
	return boost::make_shared<Coordinate>((l->getX() + r->getY()) / 2, (l->getY() + r->getY()) / 2);	
}

CoorPtr Coordinate::CalCenter(CoorPtr other)
{
	return boost::make_shared<Coordinate>((other->getX() + getY()) / 2, (other->getY() + getY()) / 2);
}