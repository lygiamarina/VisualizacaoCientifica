#include "DrawHandler.h"
#include <GL/glut.h>
#include <iostream>

DrawHandler* DrawHandler::currentInstance = NULL;

DrawHandler::DrawHandler(std::vector<Polygon> &inPolygons) : polygons(inPolygons)
{
	this->window = NULL;
}

void DrawHandler::initialize()
{
	this->window.initialize();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)this->window.getSize().width, 0.0, (GLdouble)this->window.getSize().height);

	this->polygons[0].initialize();
}

void DrawHandler::drawOrder()
{
	std::cout << "Drawing" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT);
	this->window.drawBackground();

	for(int i = 0; i < this->polygons.size(); i++)
	{
		this->polygons[i].drawPolygon();
		this->polygons[i].drawDiagonals();
	}

	glFlush();
}

void DrawHandler::drawCallback()
{
	DrawHandler::currentInstance->drawOrder();
}

void DrawHandler::draw()
{
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