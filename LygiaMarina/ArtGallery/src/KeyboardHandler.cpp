#include "KeyboardHandler.h"
#include <GL/glut.h>
#include <iostream>

KeyboardHandler* KeyboardHandler::currentInstance = NULL;

KeyboardHandler::KeyboardHandler(std::vector<Polygon> &inPolygons) : polygons(inPolygons)
{
	this->key = "";
	this->command = "";
	this->lastAction = "";
}

void KeyboardHandler::handle(unsigned char key, int x, int y)
{
	if(key == 102 || key == 70)
	{
		this->command = "Pressed";
		this->key = "F";

		this->polygons.back().getVertices().push_back(this->polygons.back().getVertices()[0]);
		this->polygons.back().setClosedPolygon(true);
		Polygon newPolygon;
		this->polygons.push_back(newPolygon);

		glutPostRedisplay();
	}
	else if(key == 116 || key == 84)
	{
		this->command = "Pressed";
		this->key = "T";

		for(int i = 0; i < this->polygons.size(); i++)
		{
			this->polygons[i].triangulate();
		}

		glutPostRedisplay();
	}
	else if(key == 112 || key == 80)
	{
		this->command = "Pressed";
		this->key = "P";

		for(int i = 0; i < this->polygons.size(); i++)
		{
			this->polygons[i].colorVertices();
		}

		glutPostRedisplay();
	}

	this->lastAction = this->key;
	this->lastAction += " ";
	this->lastAction += this->command;
	
	std::cout << this->lastAction << std::endl;	
}

void KeyboardHandler::handleCallback(unsigned char key, int x, int y)
{
	KeyboardHandler::currentInstance->handle(key, x, y);
}

void KeyboardHandler::handleKeyboard()
{
	KeyboardHandler::currentInstance = this;
	glutKeyboardFunc(KeyboardHandler::handleCallback);
}

std::string KeyboardHandler::getLastAction()
{
	return this->lastAction;
}