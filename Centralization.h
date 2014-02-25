#pragma once
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include "Coordinate.h"
#include "CommPoint.h"
#include "Coordinate.h"

class Centralization : public CommPoint
{
public:
	Centralization(boost::asio::io_service & io_service, int player_port);
	~Centralization();

protected:
	void BroadcastLocation(double x_pos, double y_pos);
	void RegisterListening();

	std::queue<CoorPtr> FilterNeighbor(int d_sense);
	std::queue<CoorPtr> FilterNeighbor(boost::unordered_map<short, CoorPtr> & other, int d_sense);
	bool CheckCenter(std::queue<CoorPtr> others, CoorPtr center, boost::function<bool(double dis)> compare);

private:
	void handle_read(unsigned char * buf,const boost::system::error_code& error,size_t bytes_transferred);
	void ParseMessage(std::string msg);

protected:
	Coordinate location_;
	
private:
	int myPort_;
	boost::unordered_map<short, CoorPtr> robotCoors_;
};