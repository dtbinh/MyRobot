#include "Manager.h"

const int defaultListenPort = 9002;
const int defautBroadCastPort = 9000;

Manager::Manager(boost::asio::io_service & io_service)
:CommPoint(io_service, defaultListenPort)
{

}

Manager::~Manager()
{
	
}