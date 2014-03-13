#pragma once

#include "Formation.h"

class Line : public Formation
{
public:
	Line(double interval, size_t formationSize);
	~Line();

	virtual CoorPtr CalcVertice(size_t index, CoorPtr source);

};