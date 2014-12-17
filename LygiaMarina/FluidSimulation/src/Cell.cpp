#include "Cell.h"
#include "math.h"

Cell::Cell()
{
	this->horizontalVelocity = 0.0;
	this->verticalVelocity = 0.0;
	this->isFluid = false;
	
	this->center = new double[2]{0.0, 0.0};
	
	this->edgesVel["Left"] = 0.0;
	this->edgesVel["Right"] = 0.0;
	this->edgesVel["Up"] = 0.0;
	this->edgesVel["Down"] = 0.0;
}

Cell::Cell(double horVelocity, double verVelocity)
{
	this->horizontalVelocity = horVelocity;
	this->verticalVelocity = verVelocity;
	this->isFluid = false;
	
	this->center = new double[2]{0.0, 0.0};
	
	this->edgesVel["Left"] = 0.0;
	this->edgesVel["Right"] = 0.0;
	this->edgesVel["Up"] = 0.0;
	this->edgesVel["Down"] = 0.0;
}

Cell::~Cell()
{
	delete [] this->center;
}

double Cell::getHorVelocity()
{
	double velLeft = this->edgesVel["Left"];
	double velRight = this->edgesVel["Right"];
	double horVel = (velLeft - velRight)/2.0;
	this->horizontalVelocity = horVel;
	return horVel;
}

double Cell::getVerVelocity()
{
	double velUp = this->edgesVel["Up"];
	double velDown = this->edgesVel["Down"];
	double verVel = (velUp - velDown)/2.0;
	this->verticalVelocity = verVel;
	return verVel;
}

double* Cell::getCenter()
{
	return this->center;
}

void Cell::setCenter(double x, double y)
{
	this->center[0] = x;
	this->center[1] = y;
}

std::map<std::string, double> Cell::getEdgesMapVel()
{
	return this->edgesVel;
}

double Cell::getEdgeVel(std::string edge)
{
	return this->edgesVel[edge];
}

void Cell::setEdgesVelMap(std::map<std::string, double> edgesVel)
{
	this->edgesVel = edgesVel;
}

void Cell::setEdgeVel(std::string edge, double velocity)
{
	
	this->edgesVel[edge] = velocity;
}

bool Cell::getIsFluidFlag()
{
	bool horVel = this->edgesVel["Left"] != 0 || this->edgesVel["Right"] != 0;
	
	bool verVel = this->edgesVel["Up"] != 0 || this->edgesVel["Up"] != 0;
	
	if (horVel || verVel)
	{
		this->setIsFluidFlag(true);
	}
	
	return this->isFluid;
}

void Cell::setIsFluidFlag(bool flag)
{
	this->isFluid = flag;
}
