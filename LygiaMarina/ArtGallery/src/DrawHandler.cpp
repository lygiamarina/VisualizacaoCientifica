#include "DrawHandler.h"
#include <GL/glut.h>

DrawHandler* DrawHandler::currentInstance = NULL;

DrawHandler::DrawHandler()
{
	this->window = NULL;
}

void DrawHandler::initialize()
{
	this->window.initialize();
	//this->vertices[0].initialize();
}

void DrawHandler::drawOrder()
{
	this->window.drawBackground();

	/*for(int i = 0; i < this->vertices.size(); i++)
	{
		this->vertices[i].drawVertex();
	}*/
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
void DrawHandler::setWindow(WindowGeneral window)
{
	this->window = window;
}

WindowGeneral DrawHandler::getWindow()
{
	return this->window;
}

void DrawHandler::setPolygonVertexVector(std::vector<PolygonVertex> vertices)
{
	this->vertices = vertices;
}

std::vector<PolygonVertex> DrawHandler::getPolygonVertexVector()
{
	return this->vertices;
}