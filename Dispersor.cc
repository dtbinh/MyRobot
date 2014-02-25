#include "Dispersor.h"

using namespace std;

Dispersor::Dispersor(boost::asio::io_service & io_service, string host, int player_port)
:LaserRobot(io_service, host, player_port)
{

}

Dispersor::~Dispersor()
{

}

void Dispersor::Run()
{

}