#ifndef POLYGON_H
#define POLYGON_H

#include "Structs.h"
#include "PolygonVertex.h"
#include <vector>

class Polygon
{
private:
	std::vector<PolygonVertex> vertices;
	std::vector<PolygonDiagonal> diagonals;
	bool closedPolygon;

public:
	Polygon();

	void triangulate();
	std::vector<PolygonDiagonal> triangulateByDiagonals(std::vector<PolygonVertex> inVertices);
	//void paintVertices();

	void initialize();
	void drawPolygon();
	void drawDiagonals();

	std::vector<PolygonVertex> &getVertices();
	std::vector<PolygonDiagonal> getDiagonals();
	void setClosedPolygon(bool state);
};

#endif