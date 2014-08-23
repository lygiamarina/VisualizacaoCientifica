#include "PolygonVertex.h"
#include <GL/glut.h>

PolygonVertex::PolygonVertex(int id)
{
	this->id = id;

	this->position = new float[2];
	this->position[0] = 0.0;
	this->position[1] = 0.0;

	this->color3Float = new float[3];
	this->color3Float[0] = 0.0;
	this->color3Float[1] = 0.0;
	this->color3Float[2] = 0.0;
}

PolygonVertex::PolygonVertex(int id, float *position, float size)
{
	this->id = id;

	this->size = size;

	this->position = position;

	this->color3Float = new float[3];
	this->color3Float[0] = 0.0;
	this->color3Float[1] = 0.0;
	this->color3Float[2] = 0.0;
}

PolygonVertex::~PolygonVertex()
{
	PolygonVertex::deleteObj();
}

//Draw methods
void PolygonVertex::drawVertex(float *color3Float)
{
	this->color3Float = color3Float;
	glColor3f(this->color3Float[0], this->color3Float[1], this->color3Float[2]);
	glBegin(GL_POINTS);
	glVertex2f(this->position[0], this->position[1]);
	glEnd();
}

//Set and Get methods
void PolygonVertex::setId(int id)
{
	this->id = id;
}

int PolygonVertex::getId()
{
	return this->id;
}

void PolygonVertex::setSize(float size)
{
	this->size = size;
}

float PolygonVertex::getSize()
{
	return this->size;
}

void PolygonVertex::setPosition(float *position)
{
	this->position = position;
}

float* PolygonVertex::getPosition()
{
	return this->position;
}

void PolygonVertex::setColor3Float(float *color3Float)
{
	this->color3Float = color3Float;
}

float* PolygonVertex::getColor3Float()
{
	return this->color3Float;
}

//Common methods
void PolygonVertex::deleteObj()
{
	delete [] this->position;
	this->position = NULL;

	delete [] this->color3Float;
	this->color3Float = NULL;
}
