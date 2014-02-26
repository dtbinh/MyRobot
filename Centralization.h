#pragma once
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include "Coordinate.h"
#include "CommPoint.h"
#include "LaserRobot.h"

const double forwardSpeed = 2;

class Centralization : public CommPoint, public LaserRobot
{
public:
	Centralization(boost::asio::io_service & io_service, std::string host, int player_port);
	~Centralization();

	void setDsense(int val);
	int getDsense();
	void setDagorithm(int val);
	int getDagorithm();
	void setInterDistance(int val);
	int getInterDistance();
	
	void Run();

protected:
	virtual bool CompareToInterRobot(CoorPtr other) = 0;
	virtual bool ComapreToCenter(CoorPtr center) = 0;
	virtual void Moving(CoorPtr center) = 0;
	virtual void Identify() = 0;

private:
	void BroadcastLocation(double x_pos, double y_pos);
	void RegisterListening();

	std::queue<CoorPtr> FilterNeighbor(int d_sense);
	std::queue<CoorPtr> FilterNeighbor(boost::unordered_map<short, CoorPtr> & other, int d_sense);
	bool CheckCenter(std::queue<CoorPtr> others, CoorPtr & center);

	void handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred);
	void handle_timerWalk(const boost::system::error_code& error);
	void ParseMessage(std::string msg);
	void Resume();
	
private:
	int myPort_;
	boost::unordered_map<short, CoorPtr> robotCoors_;
	boost::asio::deadline_timer timerWalk_;

	int d_sense_;
	int inter_distance_;
	int d_agorithm_;
};