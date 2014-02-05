#include <iostream>
#include <stdlib.h>

#include "Leader.h"
#include "Follower.h"

using namespace std;

const int defaultPort = 6665;
const string defaultHost = "localhost";

int main(int argc, char *argv[])
{
  try
  {
    string strHost = defaultHost;
  	int iPort = defaultPort;

  	if (argc >= 3)
  	{
  	  strHost = argv[1];
  	  iPort = atoi(argv[2]);
  	}
  	else if (argc >= 2)
  	{
  	  iPort = atoi(argv[1]);
  	}

    if (iPort == defaultPort)
    {
      cout << "Create a leader" << endl;
      Leader robot(strHost, iPort);
      robot.Run();
    }
    else
    {
      cout << "Create a follower" << endl;
      Follower robot(strHost, iPort);
      robot.Run();
    }
    
  }
  catch (PlayerCc::PlayerError & e)
  {
    std::cerr << e << std::endl;
    return -1;
  }
}

