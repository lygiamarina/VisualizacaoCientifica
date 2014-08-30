#include "Polygon.h"
#include <GL/glut.h>
#include <iostream>

Polygon::Polygon()
{

}

void Polygon::initialize()
{
	this->vertices[0].initialize();
}

void Polygon::drawPolygon()
{
	std::cout << "Vertices: " << this->vertices.size() << std::endl;
	for(int i = 0; i < this->vertices.size(); i++)
	{
		if (i > 0)
		{
			glLineWidth(2.5);
			glColor3f(1.0, 0.0, 0.0);

			glBegin(GL_LINES);
			glVertex2f(this->vertices[i-1].getPosition().xPosition, this->vertices[i-1].getPosition().yPosition);
			glVertex2f(this->vertices[i].getPosition().xPosition, this->vertices[i].getPosition().yPosition);
			glEnd();
		}

		this->vertices[i].drawVertex();
	}
}

void Polygon::drawDiagonals()
{
	for(int i = 0; i < this->diagonals.size(); i++)
	{
		glLineWidth(2.5);
		glColor3f(0.0, 0.0, 1.0);

		glBegin(GL_LINES);
		glVertex2f(this->diagonals[i].startVertex.xPosition, this->diagonals[i].startVertex.yPosition);
		glVertex2f(this->diagonals[i].endVertex.xPosition, this->diagonals[i].endVertex.yPosition);
		glEnd();
	}
}

std::vector<PolygonVertex> &Polygon::getVertices()
{
	return this->vertices;
}

std::vector<PolygonDiagonal> Polygon::getDiagonals()
{
	return this->diagonals;
}