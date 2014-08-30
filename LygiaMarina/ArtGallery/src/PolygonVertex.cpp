#include "PolygonVertex.h"
#include <GL/glut.h>
#include <iostream>

PolygonVertex::PolygonVertex()
{}

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

	this->position.xPosition = vertex.position.xPosition;
	this->position.yPosition = vertex.position.yPosition;

	this->color3Float.R = vertex.color3Float.R;
	this->color3Float.G = vertex.color3Float.G;
	this->color3Float.B = vertex.color3Float.B;
}

bool PolygonVertex::operator<(const PolygonVertex &vertex)
const {
	bool hasMinorY = this->position.yPosition < vertex.position.yPosition;
	bool hasSameY = this->position.yPosition == vertex.position.yPosition;
	bool hasMinorX = this->position.xPosition < vertex.position.xPosition;
	return hasMinorY || (hasSameY && hasMinorX);
}

bool PolygonVertex::operator==(const PolygonVertex &vertex)
const {
	bool hasSameY = this->position.yPosition == vertex.position.yPosition;
	bool hasSameX = this->position.xPosition == vertex.position.xPosition;
	return hasSameY && hasSameX;
}

bool PolygonVertex::operator!=(const PolygonVertex &vertex)
const {
	bool hasDifY = this->position.yPosition != vertex.position.yPosition;
	bool hasDifX = this->position.xPosition != vertex.position.xPosition;
	return hasDifY && hasDifX;
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
	glColor3f(this->color3Float.R, this->color3Float.G, this->color3Float.B);
	glBegin(GL_POINTS);
	glVertex2f(this->position.xPosition, this->position.yPosition);
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
	this->position.xPosition = xPosition;
	this->position.yPosition = yPosition;
}

FloatPosition2D PolygonVertex::getPosition()
{
	return this->position;
}

void PolygonVertex::setColor3Float(float R, float G, float B)
{
	this->color3Float.R = R;
	this->color3Float.G = G;
	this->color3Float.B = B;
}

FloatColor3D PolygonVertex::getColor3Float()
{
	return this->color3Float;
}
