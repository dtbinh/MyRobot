#pragma once
#include <queue>
#include <boost/shared_ptr.hpp>

class Coordinate;
typedef boost::shared_ptr<Coordinate> CoorPtr;

class Coordinate
{
public:
	Coordinate(double x, double y);
	~Coordinate();

	static CoorPtr CalCenter(CoorPtr l, CoorPtr r);
	CoorPtr CalCenter(std::queue<CoorPtr> others);
	CoorPtr CalCenter(CoorPtr other);

	double getDistance(CoorPtr other);
	
	double getX();
	double getY();
	void setX(double val);
	void setY(double val);

private:
	double x_;
	double y_;	
};