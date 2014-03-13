#include "FormationLeader.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9001;
const int managerBroadCastPort = 9002;

using namespace std;
using namespace boost;

FormationLeader::FormationLeader(boost::asio::io_service & io_service, std::string host, int player_port)
:LaserRobot(io_service, host, player_port),
CommPoint(io_service, defaultListenPort),
timerWalk_(io_service),
goal_(0.0, 0.0)
{

}

FormationLeader::~FormationLeader()
{

}

void FormationLeader::BroadcastLocation(string formationType, double x_pos, double y_pos)
{
	ostringstream msg;
	msg <<formationType<<" "<< x_pos <<" "<< y_pos;

	TalkToAll(msg.str(), defautBroadCastPort);
}

void FormationLeader::NotifyManager()
{
    TalkToAll(finishMsg, managerBroadCastPort);
}

void FormationLeader::Move(std::string formationType, const Coordinate & goal)
{
	//GoTo(goal->getX(), goal->getY());

    double diffY = goal.getY() - GetYPos();
    double diffX = goal.getX() - GetXPos();
    double desired_yaw = atan2(diffY, diffX) - GetYaw();

    double forwardSpeed = 0.0;
    if (formationType.compare(lineMsg) == 0)
    {
        forwardSpeed = 0.5;
    }
    else if (formationType.compare(diamondMsg) == 0)
    {
        forwardSpeed = 1;
    }

    SetSpeed(forwardSpeed, desired_yaw);
}

void FormationLeader::ParseMessage(string msg)
{
    if (msg.compare(stopMsg) == 0)
    {
        formationMsg_ = stopMsg;
        Stop();
    }
    else
    {
        vector<string> strs;
        split(strs, msg, boost::is_any_of(" "));

        if (strs.size() == 3)
        {
            if (strs[0].compare(lineMsg) == 0 || strs[0].compare(diamondMsg) == 0 )
            {
                try
                {
                    goal_.setX(lexical_cast<double>(strs[1]));
                    goal_.setY(lexical_cast<double>(strs[2]));

                    formationMsg_ = strs[0];

                    Resume(true);
                }
                catch(const bad_lexical_cast & e)
                {       
                    cerr<<"cannot parse coordinate from recv msg : "<<e.what()<<endl;
                }   
            }
            else
            {
                cerr<<"unkown recv msg : "<<endl;
            }
        }
    }
}

void FormationLeader::ParseRead(unsigned char * buf, size_t bytes_transferred)
{
	if(bytes_transferred > 0)
	{
		string msg(buf, buf + bytes_transferred);
		//cout <<"FormationLeader Receive msg: "<< msg << endl;

		ParseMessage(msg);
	}
}

void FormationLeader::Resume(bool bProcessingTask)
{
    BroadcastLocation(formationMsg_, GetXPos(), GetYPos());
    if(goal_.getDistance(make_shared<Coordinate>(GetXPos(),GetYPos())) < DistacneThreshold)
    {
        if (bProcessingTask)
        {
            NotifyManager();
            bProcessingTask = false;
        }
        
        Stop();
    }
    else
    {
        Move(formationMsg_, goal_);
    }

    timerWalk_.expires_from_now(boost::posix_time::millisec(200));
    timerWalk_.async_wait(boost::bind(&FormationLeader::handle_timerWalk, this, boost::asio::placeholders::error, bProcessingTask));
}

void Centralization::handle_timerWalk(const boost::system::error_code& error, bool bProcessingTask)
{
    if (!error)
    {
        Resume(bProcessingTask);
    }
}