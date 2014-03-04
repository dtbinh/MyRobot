#pragma once
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include "Coordinate.h"
#include "CommPoint.h"
#include "LaserRobot.h"

class Centralization : public CommPoint, public LaserRobot//, public boost::enable_shared_from_this<Centralization>
{
public:
	Centralization(boost::asio::io_service & io_service, std::string host, int player_port);
	~Centralization();

	void setDsense(double val);
	double getDsense();
	void setInterDistance(double val);
	double getInterDistance();
	
	void Run();
	void ParseRead(unsigned char * buf, size_t bytes_transferred);

protected:
	virtual bool CompareToInterRobot(CoorPtr other) = 0;
	virtual void Moving(CoorPtr center) = 0;
	virtual void Identify() = 0;

private:
	void BroadcastLocation(double x_pos, double y_pos);
	std::queue<CoorPtr> FilterNeighbor(int d_sense);
	std::queue<CoorPtr> FilterNeighbor(boost::unordered_map<int, CoorPtr> & other, int d_sense);
	CoorPtr CalcCenter(std::queue<CoorPtr> others);
	bool CheckNeighbor(std::queue<CoorPtr> others);

	void handle_timerWalk(const boost::system::error_code& error);
	void ParseMessage(std::string msg);
	void Resume();
	
private:
	int myPort_;
	boost::unordered_map<int, CoorPtr> robotCoors_;
	boost::asio::deadline_timer timerWalk_;

	double d_sense_;
	double inter_distance_;
};