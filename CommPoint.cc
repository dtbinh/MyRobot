#include "CommPoint.h"
#include "Communicate.h"

using namespace boost::asio;

CommPoint::CommPoint(io_service & io_service)
:io_service_(io_service)
{

}

CommPoint::~CommPoint()
{

}

int CommPoint::CreateListen(int port, int index)
{
  return create_listen(port, index);
}

int CommPoint::CreateBroadcast(int port, int index)
{
  return create_broadcast(port, index);
}

int CommPoint::ListenFromAll(int id, char * msg)
{
  return listen_to_robot(id, msg);
}

int CommPoint::TalkToAll(int id, int index, char * msg)
{
  return talk_to_all(id, msg, index);
}