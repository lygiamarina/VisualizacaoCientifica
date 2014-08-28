#ifndef POLYGONVERTEX_H
#define POLYGONVERTEX_H

#include "Structs.h"
#include <stdio.h>
#include <vector>

class PolygonVertex
{
private:
	int id;
	float size;
	FloatPosition2D position;
	FloatColor3D color3Float;

public:
	PolygonVertex();
	PolygonVertex(int id, float xPosition=0.0, float yPosition=0.0, float size=8.0);
	PolygonVertex(const PolygonVertex &vertex);

	//Drawing methods
	void initialize();
	void drawVertex();
	
	//Get and Set
	void setId(int id);
	int getId();

	void setSize(float size);
	float getSize();

	void setPosition(float xPosition, float yPosition);
	FloatPosition2D getPosition();

	void setColor3Float(float R, float G, float B);
	FloatColor3D getColor3Float();
};

#endif