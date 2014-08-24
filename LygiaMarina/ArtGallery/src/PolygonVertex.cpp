#include "PolygonVertex.h"
#include <GL/glut.h>
#include <iostream>

PolygonVertex::PolygonVertex(int id, float xPosition, float yPosition, float size)
{
	this->id = id;

	this->size = size;

	this->setPosition(xPosition, yPosition);

	this->setColor3Float(0.5, 0.0, 0.0);
}

PolygonVertex::PolygonVertex(const PolygonVertex &vertex)
{
	this->id = vertex.id;

	this->size = vertex.size;

	this->position = new float[2];
	*this->position = *vertex.position;
	*(this->position+1) = *(vertex.position+1);

	this->color3Float = new float[3];
	for (int i = 0; i < 3; i++)
	{
		*(this->color3Float+i) = *(vertex.color3Float+i);
	}
}

PolygonVertex::~PolygonVertex()
{
	this->deleteObj();
}

//Drawing methods
void PolygonVertex::initialize()
{
	glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void PolygonVertex::drawVertex()
{
	glPointSize(this->size);
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

void PolygonVertex::setPosition(float xPosition, float yPosition)
{
	this->position = new float[2];
	this->position[0] = xPosition;
	this->position[1] = yPosition;
}

float* PolygonVertex::getPosition()
{
	return this->position;
}

void PolygonVertex::setColor3Float(float R, float G, float B)
{
	this->color3Float = new float[3];
	this->color3Float[0] = R;
	this->color3Float[1] = G;
	this->color3Float[2] = B;
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
