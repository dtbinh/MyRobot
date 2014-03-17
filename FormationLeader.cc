#include "FormationLeader.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "Line.h"
#include "Diamond.h"

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9001;
const int managerBroadCastPort = 9002;

const double baseSpeed = 0.46;
const double speedInterval = 0.015;

using namespace std;
using namespace boost;

FormationLeader::FormationLeader(boost::asio::io_service & io_service, std::string host, int player_port)
:LaserRobot(io_service, host, player_port),
CommPoint(io_service, defaultListenPort),
timerWalk_(io_service),
goal_(0.0, 0.0)
{
    myspeed_ = baseSpeed;
    formations_[lineMsg] = make_shared<Line>(robot_interval, robot_size);
    formations_[diamondMsg] = make_shared<Diamond>(robot_interval, robot_size);
}

FormationLeader::~FormationLeader()
{

}

size_t FormationLeader::Port2Index(int port)
{
    return port - defaultPlayerPort;
}

FormationPtr FormationLeader::getFormation(string type)
{
    FormationPtr ret = formations_[type];
    formations_[stopMsg] = ret;

    return ret;
}

void FormationLeader::BroadcastLocation(string formationType, double x_pos, double y_pos, double speed, double yaw)
{
	ostringstream msg;
	msg <<formationType<<" "<< x_pos <<" "<< y_pos<<" "<<speed<<" "<<yaw;

	TalkToAll(msg.str(), defautBroadCastPort);
}

void FormationLeader::NotifyManager()
{
    TalkToAll(finishMsg, managerBroadCastPort);
}

void FormationLeader::ParseMessage(string msg)
{
    if (msg.compare(stopMsg) == 0)
    {
        cout <<"FormationLeader Receive msg: "<< msg << endl;

        formationMsg_ = stopMsg;
        Stop();
    }
    else
    {
        vector<string> strs;
        split(strs, msg, boost::is_any_of(" "));

        if (strs.size() == 3)
        {
            cout <<"FormationLeader Receive msg: "<< msg << endl;

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
        else
        {
            vector<string> strs;
            split(strs, msg, boost::is_any_of(":"));

            if (strs.size() == 2)
            {
                int port = 0;
                try
                {
                    port = lexical_cast<int>(strs[0]);
                }
                catch(const bad_lexical_cast & e)
                {
                    cerr<<"cannot parse player port from recv msg : "<<e.what()<<endl;
                    return;
                }

                vector<string> coors;
                split(coors, strs[1], boost::is_any_of(","));
                if (coors.size() == 2)
                {
                    try
                    {
                        double x = lexical_cast<double>(coors[0]);
                        double y = lexical_cast<double>(coors[1]);
                        AdjustSpeed(port,make_shared<Coordinate>(x,y));
                    }
                    catch(const bad_lexical_cast & e)
                    {       
                        cerr<<"cannot parse coordinate from recv msg : "<<e.what()<<endl;
                    }
                }
            }
        }
    }
}

void FormationLeader::ParseRead(unsigned char * buf, size_t bytes_transferred)
{
	if(bytes_transferred > 0)
	{
		string msg(buf, buf + bytes_transferred);

		ParseMessage(msg);
	}
}

void FormationLeader::Speeder(double limitation)
{
    if (myspeed_ < limitation)
    {
        myspeed_ += speedInterval;
    }
}

void FormationLeader::Slowdown(double limitation)
{
    if (myspeed_ > limitation)
    {
        myspeed_ -= speedInterval;
    }
}

void FormationLeader::AdjustSpeed(int port, CoorPtr follower)
{
    FormationPtr formation = getFormation(formationMsg_);
    CoorPtr destination = formation->CalcVerticeToLeader(Port2Index(port), make_shared<Coordinate>(GetXPos(), GetYPos()));

    double distance = follower->getDistance(destination);

    if (distance > DistacneThreshold)
    {
        //cout<<"follower to destination = "<<distance<<endl;

        if (follower->getX() < destination->getX())
        {
            Slowdown(baseSpeed / 2);
        }
        else if (follower->getX() > destination->getX())
        {
            Speeder(2 * baseSpeed);
        }
    }
    else
    {
        Speeder(baseSpeed);
    }
}

void FormationLeader::Move(std::string formationType, Coordinate & goal)
{
    // player_pose2d_t pos;
    // pos.px = goal.getX();
    // pos.py = goal.getY();
    // GoTo(pos, pos);   

    double diffY = goal.getY() - GetYPos();
    double diffX = goal.getX() - GetXPos();
    double desired_yaw = atan2(diffY, diffX) - GetYaw();
    SetSpeed(myspeed_, desired_yaw / comm_interval);
}

void FormationLeader::Resume(bool bProcessingTask)
{   
    //cout<<"Self Position : "<<GetXPos()<<" "<<GetYPos()<<" "<<GetXSpeed()<<" "<<GetYaw()<<endl;
    double comm_offset = 0.375;
    if (formationMsg_.compare(stopMsg) == 0)
    {
        comm_offset = 0.0;
    }

    BroadcastLocation(formationMsg_, GetXPos() + comm_offset, GetYPos(), GetXSpeed(), GetYaw());
    
    if(goal_.getDistance(make_shared<Coordinate>(GetXPos(),GetYPos())) < DistacneThreshold)
    {
        if (bProcessingTask)
        {
            NotifyManager();
            bProcessingTask = false;
        }
        
        //GoTo(goal_.getX(), goal_.getY());
        Stop();
    }
    else
    {
        Move(formationMsg_, goal_);
    }

    timerWalk_.expires_from_now(boost::posix_time::millisec(comm_interval));
    timerWalk_.async_wait(boost::bind(&FormationLeader::handle_timerWalk, this, boost::asio::placeholders::error, bProcessingTask));
}

void FormationLeader::handle_timerWalk(const boost::system::error_code& error, bool bProcessingTask)
{
    if (!error)
    {
        Read();
        Resume(bProcessingTask);
    }
}

void FormationLeader::Run()
{
    cout << "FormationLeader is running" << endl;

    Read();
    ListenFromAll();
}