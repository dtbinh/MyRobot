#include "Centralization.h"
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9001;

using namespace std;
using namespace boost;

Centralization::Centralization(boost::asio::io_service & io_service, std::string host, int player_port)
:CommPoint(io_service, defaultListenPort),
LaserRobot(io_service, host, player_port),
timerWalk_(io_service),
myPort_(player_port)
{

}

Centralization::~Centralization()
{

}

queue<CoorPtr> Centralization::FilterNeighbor(int d_sense)
{
	return FilterNeighbor(robotCoors_, d_sense);
}

queue<CoorPtr> Centralization::FilterNeighbor(unordered_map<short, CoorPtr> & others, int d_sense)
{
	queue<CoorPtr> ret;

	Coordinate location(GetXPos(), GetYPos());

	for(unordered_map<short, CoorPtr>::iterator it = others.begin(); it != others.end(); it++)
	{
		if (location.getDistance(it->second) <= d_sense)
		{
			ret.push(it->second);
		}
	}

	return ret;
}

void Centralization::BroadcastLocation(double x_pos, double y_pos)
{
	ostringstream msg;
	msg << myPort_ <<":"<< x_pos <<","<< y_pos;

	TalkToAll(msg.str(), defautBroadCastPort);
}

void Centralization::RegisterListening()
{
	ListenFromAll(boost::bind(&Centralization::handle_read, this, _1, _2, _3));	
}

void Centralization::ParseMessage(string msg)
{
	vector<string> strs;
	split(strs, msg, boost::is_any_of(":"));

	if (strs.size() == 2)
	{
		short port = myPort_;
		try
    	{
        	port = lexical_cast<short>(strs[0]);
    	}
    	catch(const bad_lexical_cast & e)
    	{
        	cerr<<"cannot parse player port from recv msg : "<<e.what()<<endl;
        	return;
    	}

    	if (port != myPort_)
    	{
    		vector<string> coors;
    		split(coors, strs[1], boost::is_any_of(","));
    		if (coors.size() == 2)
    		{
    			try
    			{
    				double x = lexical_cast<double>(coors[0]);
    				double y = lexical_cast<double>(coors[1]);
    				robotCoors_[port] = make_shared<Coordinate>(x,y);
    			}
    			catch(const bad_lexical_cast & e)
	        	{		
	            	cerr<<"cannot parse coordinate from recv msg : "<<e.what()<<endl;
	        	}
    		}
    	}
	}
}

void Centralization::handle_read(unsigned char * buf, const boost::system::error_code& error, size_t bytes_transferred)
{
	if(!error && bytes_transferred > 0)
	{
		string msg(buf, buf + bytes_transferred);
		cout <<"Receive msg: "<< msg << endl;

		ParseMessage(msg);
	}

	ListenFromAll(boost::bind(&Centralization::handle_read, this, _1, _2, _3));
}

int Centralization::getDsense()
{
	return d_sense_;
}

void Centralization::setDsense(int val)
{
	d_sense_ = val;
}

int Centralization::getDagorithm()
{
	return d_agorithm_;
}

void Centralization::setDagorithm(int val)
{
	d_agorithm_ = val;
}

void Centralization::setInterDistance(int val)
{
	inter_distance_ = val;
}

int Centralization::getInterDistance()
{
	return inter_distance_;
}

bool Centralization::CheckCenter(queue<CoorPtr> others, CoorPtr center)
{
	Coordinate location(GetXPos(), GetYPos());
	center = location.CalCenter(others);

	while(!others.empty())
	{
		CoorPtr other = others.front();
		others.pop();

		if (CompareToInterRobot(other))
		{
			return true;
		}
	}

	return false;
}

void Centralization::Resume()
{
	LaserAvoidance();
	BroadcastLocation(GetXPos(), GetYPos());
	
	CoorPtr center;
	if(CheckCenter(FilterNeighbor(getDsense()), center) 
		&& ComapreToCenter(center))
	{
		Moving(center);
	}
	else
	{
		Stop();
	}

	timerWalk_.expires_from_now(boost::posix_time::millisec(200));
    timerWalk_.async_wait(boost::bind(&Centralization::handle_timerWalk, this, boost::asio::placeholders::error));
}

void Centralization::handle_timerWalk(const boost::system::error_code& error)
{
	if (!error)
 	{
 		Resume();
 	}
}

void Centralization::Run()
{
	Identify();
	
	RegisterListening();

	Resume();
}