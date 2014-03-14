#pragma once
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp> 

class Coordinate;
typedef boost::shared_ptr<Coordinate> CoorPtr;

class Coordinate : public boost::enable_shared_from_this<Coordinate>
{
public:
	Coordinate(double x, double y);
	~Coordinate();

	static CoorPtr CalCenter(CoorPtr l, CoorPtr r);
	CoorPtr CalCenter(std::queue<CoorPtr> others);
	CoorPtr CalCenter(CoorPtr other);

	double getDistance(CoorPtr other);
	double getDistance(double x_pos, double y_pos);
	
	double getX();
	double getY();
	void setX(double val);
	void setY(double val);

private:
	double x_;
	double y_;	
};