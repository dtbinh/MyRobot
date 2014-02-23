#include <iostream>
#include <stdlib.h>

#include "CommPoint.h"

using namespace std;

int main(int argc, char *argv[])
{
  try
  {
    int iRecv = atoi(argv[1]);
    int iSend = atoi(argv[2]);

    boost::asio::io_service io_service_;
    boost::asio::io_service::work work_(io_service_);

    CommPoint comm(io_service_, iRecv, "10.0.2.15");
    ostringstream ostr;
    ostr<<" send to port: "<<iSend;

    comm.ListenFromAll();
    if(iSend == 9001)
    {
      comm.TalkToAll(ostr.str(), iSend);
    }



    io_service_.run();
    
    cout << "Exit" << endl;
    
  }
  catch (std::exception& e)
  {
    std::cerr << "main catch exception: " << e.what() << std::endl;  
    return -1;
  }
  catch (...)
  {
    cerr<< "Unknown type error"<<endl;
    return -1;
  }
}