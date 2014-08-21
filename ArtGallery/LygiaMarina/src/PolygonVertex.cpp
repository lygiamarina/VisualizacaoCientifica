#include <PolygonVertex.h>

PolygonVertex::PolygonVertex(int id)
{
	this.id = id;
}

PolygonVertex::PolygonVertex(int id, pair <float, float> position, float size=1.0f)
{
	this.id = id;
	this.position = position;
	this.size = size;
}

//Draw methods
void PolygonVertex::drawVertexWithoutColor()
{
	glBegin(GL_POINTS)
	glVertex2f(this.position.first, this.position.second)
	glEnd()
}

void PolygonVertex::drawVertexWithColor()
{
	glColor3f(get<0>(this.color3Float), get<1>(this.color3Float), get<2>(this.color3Float));
	glBegin(GL_POINTS)
	glVertex2f(this.position.first, this.position.second)
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

void PolygonVertex::setPosition(pair <float, float> position)
{
	this.position = position;
}

pair <float, float> PolygonVertex::getPosition()
{
	return this.position;
}

void PolygonVertex::setColor3Float(tuple <float, float, float> color3Float)
{
	this.color3Float = color3Float;
}

tuple <float, float, float> PolygonVertex::getColor3Float()
{
	return this.color3Float;
}
