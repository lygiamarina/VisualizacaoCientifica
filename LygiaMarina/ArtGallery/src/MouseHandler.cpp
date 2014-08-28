#include "MouseHandler.h"
#include <GL/glut.h>
#include <iostream>

MouseHandler* MouseHandler::currentInstance = NULL;

MouseHandler::MouseHandler()
{
	this->keyName = "";
	this->command = "";
	this->lastAction = "";
}

void MouseHandler::handle(int button, int state, int x, int y)
{
	this->mousePosition.xPosition = x;
	this->mousePosition.yPosition = y;

	if(state == GLUT_DOWN)
	{
		this->command = "Pressed";
		
		if (button == GLUT_LEFT_BUTTON) 
		{
			this->keyName = "Left Key";
		}
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			this->keyName = "Scroll Key";
		}
		else 
		{
			this->keyName = "Right Key";
		}
	}
	else
	{
		this->command = "Released";
		if (button == GLUT_LEFT_BUTTON) 
		{
			this->keyName = "Left Key";
		}
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			this->keyName = "Scroll Key";
		}
		else 
		{
			this->keyName = "Right Key";
		}
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

std::string MouseHandler::getLastAction()
{
	return this->lastAction;
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
