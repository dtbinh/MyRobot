#pragma once

const int defaultPlayerPort = 6665;

class Robot
{
public:
	Robot();
	~Robot();

	virtual void Run() = 0;
	
};