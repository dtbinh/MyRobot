#include "FormationLeader.h"

FormationLeader::FormationLeader(boost::asio::io_service & io_service, std::string host, int player_port)
:LaserRobot(io_service, host, player_port)
{

}

FormationLeader::~FormationLeader()
{

}