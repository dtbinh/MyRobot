#pragma once

#include "Formation.h"

class Line : public Formation
{
public:
	Line(double interval, size_t formationSize);
	~Line();

	virtual CoorPtr CalcVerticeToLeader(size_t index, CoorPtr source);

protected:
	virtual double CalcIntervalToLeader(size_t index);
};