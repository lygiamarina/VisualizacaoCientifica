#include "DrawHandler.h"
#include <GL/glut.h>
#include <iostream>

DrawHandler* DrawHandler::currentInstance = NULL;

DrawHandler::DrawHandler(std::vector<PolygonVertex> &inVertices) : vertices(inVertices)
{
	this->window = NULL;
}

void DrawHandler::initialize()
{
	this->window.initialize();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)this->window.getSize().width, 0.0, (GLdouble)this->window.getSize().height);

	this->vertices[0].initialize();
}

void DrawHandler::drawOrder()
{
	std::cout << "Desenhando 3" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT);
	this->window.drawBackground();

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

		/*std::cout << "ID: " << this->vertices[i].getId() << std::endl;
		std::cout << "xPosition: " << this->vertices[i].getPosition().xPosition << std::endl;
		std::cout << "yPosition: " << this->vertices[i].getPosition().yPosition << std::endl;*/

	}

	std::cout << "" << std::endl;

	glFlush();
}

void DrawHandler::drawCallback()
{
	std::cout << "Desenhando 2" << std::endl;
	DrawHandler::currentInstance->drawOrder();
}

void DrawHandler::draw()
{
	std::cout << "Desenhando 1" << std::endl;
	DrawHandler::currentInstance = this;
	glutDisplayFunc(DrawHandler::drawCallback);
}

//Set and Get
void DrawHandler::setWindow(WindowGeneral &window)
{
	this->window = window;
}

WindowGeneral DrawHandler::getWindow()
{
	return this->window;
}