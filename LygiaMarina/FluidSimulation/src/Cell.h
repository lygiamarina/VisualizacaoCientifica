#ifndef CELL_H
#define CELL_H

#include <vector>
#include <map>
#include <string>

class Cell
{
private:
	double *center;
	double horizontalVelocity;
	double verticalVelocity;
	std::map<std::string, double> edgesVel;
	bool isFluid;
	
public:
	Cell();
	Cell(double horVelocity, double verVelocity);
	~Cell();
	
	Cell& operator=(const Cell& inCell);
	
	double getHorVelocity();
	
	double getVerVelocity();
	
	double* getCenter();
	void setCenter(double x, double y);
	
	std::map<std::string, double> getEdgesMapVel();
	double getEdgeVel(std::string edge);
	void setEdgesVelMap(std::map<std::string, double> edgesVel);
	void setEdgeVel(std::string edge, double velocity);
	
	bool getIsFluidFlag();
	void setIsFluidFlag(bool flag);
};
#endif
