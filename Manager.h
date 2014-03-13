#pragma once
#include "CommPoint.h"

class Manager : public CommPoint
{
public:
	Manager(boost::asio::io_service & io_service);
	~Manager();
	
};