#pragma once
#include "Formation.h"

class Diamond : public Formation
{
public:
	Diamond(double interval, size_t formationSize);
	~Diamond();

	CoorPtr CalcVertice(size_t index, CoorPtr source);

};