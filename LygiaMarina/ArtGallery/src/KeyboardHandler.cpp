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
	if(key == 'f' || key == 'F')
	{
		this->command = "Pressed";
		this->key = "F";

		if(this->polygons.back().getVertices().size())
		{
			this->polygons.back().getVertices().push_back(this->polygons.back().getVertices()[0]);
			this->polygons.back().setClosedPolygon(true);
			Polygon newPolygon;
			this->polygons.push_back(newPolygon);

			glutPostRedisplay();
		}
	}
	else if(key == 't' || key == 'T')
	{
		this->command = "Pressed";
		this->key = "T";

		for(int i = 0; i < this->polygons.size(); i++)
		{
			this->polygons[i].triangulate();
		}

		glutPostRedisplay();
	}
	else if(key == 'p' || key == 'P')
	{
		this->command = "Pressed";
		this->key = "P";

		for(int i = 0; i < this->polygons.size(); i++)
		{
			this->polygons[i].colorVertices();
		}

		glutPostRedisplay();
	}
	else if(key == 'c' || key == 'C')
	{
		this->command = "Pressed";
		this->key = "C";

		this->polygons.clear();

		Polygon newPolygon;

		this->polygons.push_back(newPolygon);

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