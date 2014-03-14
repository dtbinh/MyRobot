#include "FormationFollower.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include "Line.h"
#include "Diamond.h"

const int defaultListenPort = 9001;
const int defautBroadCastPort = 9000;

using namespace std;
using namespace boost;

const double robot_interval = 1.0;
const size_t robot_size = 4;

FormationFollower::FormationFollower(boost::asio::io_service & io_service, std::string host, int player_port)
:LaserRobot(io_service, host, player_port),
CommPoint(io_service, defaultListenPort),
MyPort_(player_port)
{
    formations_[lineMsg] = make_shared<Line>(robot_interval, robot_size);
    formations_[diamondMsg] = make_shared<Diamond>(robot_interval, robot_size);
}

FormationFollower::~FormationFollower()
{
	
}

size_t FormationFollower::Port2Index(int port)
{
    return port - defaultPlayerPort;
}

FormationPtr FormationFollower::getFormation(string type)
{
    FormationPtr ret = formations_[type];
    formations_[stopMsg] = ret;

    return ret;
}

void FormationFollower::Movement(std::string formationType, CoorPtr leadr)
{
    CoorPtr destination = getFormation(formationType)->CalcVertice(Port2Index(MyPort_), leadr);
    double distacne = destination->getDistance(make_shared<Coordinate>(GetXPos(),GetYPos()));
    if(distacne < DistacneThreshold)
    {
        Stop();
    }
    else
    {
        Move(destination);    
    }
}

void FormationFollower::Move(CoorPtr destination)
{
    double diffY = destination->getY() - GetYPos();
    double diffX = destination->getX() - GetXPos();
    double desired_yaw = atan2(diffY, diffX);

    double min_yaw = math::constants::pi<double>() / -2;
    double max_yaw = math::constants::pi<double>() / 2;
    if (desired_yaw < min_yaw)
    {
        desired_yaw = min_yaw;
    }
    else if (desired_yaw > max_yaw)
    {
        desired_yaw = max_yaw;
    }

    double distance = destination->getDistance(GetYPos(), GetYPos());
    if (distance > robot_interval)
    {
        SetSpeed(baseSpeed * 2, desired_yaw - GetYaw());
    }
    else 
    {
        SetSpeed(baseSpeed, desired_yaw - GetYaw());
    }    
}

void FormationFollower::ParseMessage(string msg)
{
    vector<string> strs;
    split(strs, msg, boost::is_any_of(" "));

    if (strs.size() == 3)
    {
        if (strs[0].compare(lineMsg) == 0 || strs[0].compare(diamondMsg) == 0 || strs[0].compare(stopMsg) == 0)
        {
            try
            {
                double x = lexical_cast<double>(strs[1]);
                double y = lexical_cast<double>(strs[2]);

                Movement(strs[0], make_shared<Coordinate>(x,y));
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

void FormationFollower::ParseRead(unsigned char * buf, size_t bytes_transferred)
{
	if(bytes_transferred > 0)
	{
		string msg(buf, buf + bytes_transferred);
		cout <<"FormationFollower Receive msg: "<< msg << endl;

		ParseMessage(msg);
	}
}

void FormationFollower::Run()
{
    cout << "FormationFollower is running" << endl;

    ListenFromAll();
}