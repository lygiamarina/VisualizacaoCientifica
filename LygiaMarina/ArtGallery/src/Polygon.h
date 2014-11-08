#ifndef POLYGON_H
#define POLYGON_H

#include "Structs.h"
#include "VertexStructs.h"
#include "PolygonVertex.h"
#include <vector>
#include <deque>

class Polygon
{
private:
	std::vector<PolygonVertex> vertices;
	std::vector<PolygonDiagonal> diagonals;
	std::deque<PolygonTriangle> triangles; 
	bool closedPolygon;
	bool triangulatedPolygon;
	bool verticesPaintedPolygon;

public:
	Polygon();

	void triangulate();
	std::deque<PolygonTriangle> triangulateByDiagonals(std::vector<PolygonVertex> inVertices);
	void setCounterClockwise();
	void colorVertices();
	void paintVertices();

	void initialize();
	void drawPolygon();
	void drawDiagonals();

	std::vector<PolygonVertex> &getVertices();
	std::vector<PolygonDiagonal> getDiagonals();
	void setClosedPolygon(bool state);
	void setTriangulatedPolygon(bool state);
	void setVerticesPaintedPolygon(bool state);
};

#endif