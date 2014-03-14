#include <iostream>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "Manager.h"

using namespace std;

const string strDefault= "default";
const string strQuit = "quit";

int DefaultWaypoints(Manager & mng)
{
	mng.AddWayPoint(-11.05, 0.09);

	mng.AddWayPoint(0.05, 0.09);

	mng.AddWayPoint(10.05, -0.09);	
}

int ParseInput(string & msg, Manager & mng)
{
	using namespace boost;

	vector<string> strs;
    split(strs, msg, boost::is_any_of(" "));

    if (strs.size() == 2)
    {
        try
        {
            double x = lexical_cast<double>(strs[0]);
            double y = lexical_cast<double>(strs[1]);

            mng.AddWayPoint(x, y);
        }
        catch(const bad_lexical_cast & e)
        {       
            cerr<<"cannot parse coordinate from recv msg : "<<e.what()<<endl;
        }  
    } 
    else
    {
        cerr<<"invalid recv msg : "<<endl;
    }
}

int GetInput(Manager & mng)
{
	cout<<"input a waypoint as \"x.xx y.yy\", type \""<<strQuit<<"\" to end input, type \""<<strDefault<<"\" to add three default waypoints"<<endl;

	string input;
	cin >> input;

	if (input.compare(strQuit) == 0)
	{
		return 0;
	}
	else if (input.compare(strDefault) == 0)
	{
		return DefaultWaypoints(mng);
	}

	ParseInput(input, mng);

	return GetInput(mng);
}

int main(int argc, char *argv[])
{
	boost::asio::io_service io_service_;
	boost::asio::io_service::work work_(io_service_);

	Manager mng(io_service_);

	if (argc == 2 && string(argv[1]).compare(strDefault) == 0)
	{
		DefaultWaypoints(mng);
	}
	else
	{
		GetInput(mng);
	}

	mng.Run();

	io_service_.run();

	cout << "Robot Exit" << endl;
}