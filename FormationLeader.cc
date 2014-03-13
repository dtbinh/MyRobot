#include "FormationLeader.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

const int defaultListenPort = 9000;
const int defautBroadCastPort = 9001;

using namespace std;
using namespace boost;

FormationLeader::FormationLeader(boost::asio::io_service & io_service, std::string host, int player_port)
:LaserRobot(io_service, host, player_port),
CommPoint(io_service, defaultListenPort)
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

void FormationLeader::Movement(std::string formationType, CoorPtr goal)
{
	GoTo(goal->getX(), goal->getY());
}

void FormationLeader::ParseMessage(string msg)
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

                formationMsg_ = strs[0];

                BroadcastLocation(formationMsg_, GetXPos(), GetYPos());
                Movement(formationMsg_, make_shared<Coordinate>(x,y));
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

void FormationLeader::ParseRead(unsigned char * buf, size_t bytes_transferred)
{
	if(bytes_transferred > 0)
	{
		string msg(buf, buf + bytes_transferred);
		//cout <<"FormationLeader Receive msg: "<< msg << endl;

		ParseMessage(msg);
	}
}