#include "PolygonVertex.h"
#include <GL/glut.h>

PolygonVertex::PolygonVertex(int id)
{
	this.id = id;
	this.position = {0,0};
	this.color3Float = {0,0,0};
}

PolygonVertex::PolygonVertex(int id, float *position, float size=1.0f)
{
	this.id = id;
	this.size = size;
	this.position[0] = *position;
	this.position[1] = *(position+1);
	this.color3Float = {0,0,0};
}

//Draw methods
void PolygonVertex::drawVertex(float *color3Float)
{
	for (int i = 0; i < 3; i++)
	{
		this.color3Float[i] = *(color3Float + i);
	}
	
	glColor3f(this.color3Float[0], this.color3Float[1], this.color3Float[2]);
	glBegin(GL_POINTS)
	glVertex2f(this.position[0], this.position[1])
	glEnd()
}

//Set and Get methods
void PolygonVertex::setId(int id)
{
	this.id = id;
}

int PolygonVertex::getId()
{
	return this.id;
}

void PolygonVertex::setSize(float size)
{
	this.size = size;
}

float PolygonVertex::getSize()
{
	return this.size;
}

void PolygonVertex::setPosition(float *position)
{
	this.position[0] = *position;
	this.position[1] = *(position+1);
}

float* PolygonVertex::getPosition()
{
	return this.position;
}

void PolygonVertex::setColor3Float(float *color3Float)
{
	for (int i = 0; i < 3; i++)
	{
		this.color3Float[i] = *(color3Float + i);
	}
}

float* PolygonVertex::getColor3Float()
{
	return this.color3Float;
}
