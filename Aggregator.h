#pragma once
#include "LaserRobot.h"
#include "Centralization.h"

class Aggregator : public LaserRobot, public Centralization
{
public:
	Aggregator(boost::asio::io_service & io_service, std::string host, int player_port);
	~Aggregator();

	virtual void Run();
	void setDsense(int val);
	int getDsense();
	void setDaggregate(int val);
	int getDaggregate();
	void setInterDistance(int val);
	int getInterDistance();

private:
	void handle_timerWalk(const boost::system::error_code& error);
	bool compare(double distance);
	void Resume();
	void MoveTowards(CoorPtr destination);
	
private:
	boost::asio::deadline_timer timerWalk_;
	int d_sense_;
	int inter_distance_;
	int d_aggregate_;
};