#include "DrawHandler.h"
#include <GL/glut.h>
#include <iostream>

DrawHandler* DrawHandler::currentInstance = NULL;

DrawHandler::DrawHandler()
{
	this->window = NULL;
}

DrawHandler::~DrawHandler()
{
	this->window.deleteObj();
	for(int i = 0; i < this->vertices.size(); i++)
	{
		this->vertices[i].deleteObj();
	}
}

void DrawHandler::initialize()
{
	this->window.initialize();
	this->vertices[0].initialize();
}

void DrawHandler::drawOrder()
{
	std::cout << "Desenhando 3" << std::endl;
	glClear(GL_COLOR_BUFFER_BIT);
	this->window.drawBackground();

	for(int i = 0; i < this->vertices.size(); i++)
	{
		this->vertices[i].drawVertex();
		std::cout << "ID: " << this->vertices[i].getId() << std::endl;
		std::cout << "xPosition: " << this->vertices[i].getPosition()[0] << std::endl;
		std::cout << "yPosition: " << this->vertices[i].getPosition()[1] << std::endl;
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
	glutMainLoop();
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

void DrawHandler::setPolygonVertexVector(std::vector<PolygonVertex> vertices)
{
	this->vertices = vertices;
}

std::vector<PolygonVertex> DrawHandler::getPolygonVertexVector()
{
	return this->vertices;
}