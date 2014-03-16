#pragma once
#include "Formation.h"

class Diamond : public Formation
{
public:
	Diamond(double interval, size_t formationSize);
	~Diamond();

	virtual CoorPtr CalcVerticeToLeader(size_t index, CoorPtr source);

protected:
	virtual double CalcIntervalToLeader(size_t index);
};