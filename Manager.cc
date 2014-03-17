#include "Manager.h"

const int defaultListenPort = 9002;
const int defautBroadCastPort = 9000;

using namespace std;

Manager::Manager(boost::asio::io_service & io_service)
:CommPoint(io_service, defaultListenPort),
curPt_(boost::posix_time::microsec_clock::local_time())
{

}

Manager::~Manager()
{
	
}

int Manager::AddWayPoint(double x, double y)
{
	return AddWayPoint(Coordinate(x,y));
}

int Manager::AddWayPoint(Coordinate waypoint)
{
	string newCmd = "";

	if (waypoints_.empty())
	{
		newCmd = lineMsg;
	}
	else
	{
		if(waypoints_.back().formationType_.compare(lineMsg) == 0)
		{
			newCmd = diamondMsg;
		}
		else
		{
			newCmd = lineMsg;
		}
	}

	stSection newWaypoint(newCmd, waypoint);
	waypoints_.push(newWaypoint);
}

void Manager::SendCmd()
{
	ostringstream msg;

	if (waypoints_.empty())
	{
		msg << stopMsg;
	}
	else
	{
		curSec_ = waypoints_.front();
		waypoints_.pop();

		msg <<curSec_.formationType_<<" "<< curSec_.waypoint_.getX() <<" "<< curSec_.waypoint_.getY();
	}

	TalkToAll(msg.str(), defautBroadCastPort);
	cout<<"Send Cmd : \""<<msg.str()<<"\""<<endl;

	RecordTime();
}

void Manager::ParseRead(unsigned char * buf, size_t bytes_transferred)
{
	if(bytes_transferred > 0)
	{
		string msg(buf, buf + bytes_transferred);
		//cout <<"Manager Receive msg: "<< msg << endl;

		if (msg.compare(finishMsg) == 0)
		{
			Report();

			SendCmd();
		}
	}
}

void Manager::RecordTime()
{
	curPt_ = boost::posix_time::microsec_clock::local_time();
}

void Manager::Report()
{
	boost::posix_time::time_duration msdiff = boost::posix_time::microsec_clock::local_time() - curPt_;
	
	cout<<"Mission \""<<curSec_.formationType_<<" to ("<<curSec_.waypoint_.getX()<<", "<<curSec_.waypoint_.getY()<<")\""
	<<" accomplished in "<<msdiff.total_milliseconds()<<" milliseconds"<<endl;
}

void Manager::Run()
{
	cout << "Manager is running" << endl;
	
	ListenFromAll();

	SendCmd();
}