#include "Centralization.h"
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9000;

const double forwardSpeed = 0.5;

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

	//cout<<"Recv Robot Num = "<<robotCoors_.size()<<endl;
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

void Centralization::ParseRead(unsigned char * buf, size_t bytes_transferred)
{
	if(bytes_transferred > 0)
	{
		string msg(buf, buf + bytes_transferred);
		//cout <<"Centralization Receive msg: "<< msg << endl;

		ParseMessage(msg);
	}
}

double Centralization::getDsense()
{
	return d_sense_;
}

void Centralization::setDsense(double val)
{
	d_sense_ = val;
}

void Centralization::setInterDistance(double val)
{
	inter_distance_ = val;
}

double Centralization::getInterDistance()
{
	return inter_distance_;
}

CoorPtr Centralization::CalcCenter(queue<CoorPtr> others)
{
	Coordinate location(GetXPos(), GetYPos());
	return location.CalCenter(others);
}

bool Centralization::CheckNeighbor(queue<CoorPtr> others)
{
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
	BroadcastLocation(GetXPos(), GetYPos());
	
	queue<CoorPtr> neighbor = FilterNeighbor(getDsense());

	if(CheckNeighbor(neighbor))
	{
		CoorPtr center = CalcCenter(neighbor);
		Moving(center);
	}
	else
	{
		Stop();
	}

	timerWalk_.expires_from_now(boost::posix_time::millisec(200));
    timerWalk_.async_wait(boost::bind(&Centralization::handle_timerWalk, this/*shared_from_this()*/, boost::asio::placeholders::error));
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
	
	ListenFromAll();

	double newspeed;
	double newturnrate;
	SetSpeed(forwardSpeed, 0.0);
	
	LaserAvoidance(newspeed, newturnrate);
	SetSpeed(newspeed, newturnrate);

	Resume();
}