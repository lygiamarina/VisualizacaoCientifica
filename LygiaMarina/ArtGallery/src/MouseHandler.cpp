#include "MouseHandler.h"
#include <GL/glut.h>
#include <iostream>

MouseHandler* MouseHandler::currentInstance = NULL;

MouseHandler::MouseHandler(std::vector<PolygonVertex> &inVertices, int height) : vertices(inVertices)
{
	this->keyName = "";
	this->command = "";
	this->lastAction = "";
	this->windowHeight = height;
}

void MouseHandler::handle(int button, int state, int x, int y)
{
	this->mousePosition.xPosition = x;
	this->mousePosition.yPosition = this->windowHeight - y;

	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		this->command = "Pressed";
		this->keyName = "Left key";

		PolygonVertex localVertex(currentVertexId, this->mousePosition.xPosition, this->mousePosition.yPosition);
		this->vertices.push_back(localVertex);
		this->currentVertexId++;

		glutPostRedisplay();
	}

	this->lastAction = this->keyName;
	this->lastAction += " ";
	this->lastAction += this->command;
	
	std::cout << this->lastAction << std::endl;	
	
	std::cout << "X: " << this->mousePosition.xPosition << std::endl;	
	std::cout << "Y: " << this->mousePosition.yPosition << std::endl;	
}

void MouseHandler::handleCallback(int button, int state, int x, int y)
{
	MouseHandler::currentInstance->handle(button, state, x, y);
}

void MouseHandler::handleMouse()
{
	MouseHandler::currentInstance = this;
	glutMouseFunc(MouseHandler::handleCallback);
}

void MouseHandler::setMousePosition(int xPosition, int yPosition)
{
	this->mousePosition.xPosition = xPosition;
	this->mousePosition.yPosition = yPosition;
}

IntPosition2D MouseHandler::getMousePosition()
{
	return this->mousePosition;
}

void MouseHandler::setCurrentVertexId(int id)
{
	this->currentVertexId = id;
}

std::string MouseHandler::getLastAction()
{
	return this->lastAction;
}