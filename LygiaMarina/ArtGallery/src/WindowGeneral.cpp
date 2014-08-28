#include "WindowGeneral.h"
#include <GL/glut.h>
#include <string>
#include <iostream>

WindowGeneral::WindowGeneral(char *title)
{
	this->title = title;

	this->setSize(600, 480);

	this->setPosition(50, 25);

	this->setBackgroundColor4Float(0.95, 0.95, 0.95, 1.0);
}

WindowGeneral::WindowGeneral(const WindowGeneral &window)
{
	this->title = window.title;

	this->size.width = window.size.width;
	this->size.height = window.size.height;

	this->position.xPosition = window.position.xPosition;
	this->position.yPosition = window.position.yPosition;

	this->backgroundColor4Float.R = window.backgroundColor4Float.R;
	this->backgroundColor4Float.G = window.backgroundColor4Float.G;
	this->backgroundColor4Float.B = window.backgroundColor4Float.B;
	this->backgroundColor4Float.alpha = window.backgroundColor4Float.alpha;
}

//Screen
void WindowGeneral::initialize()
{
	//Simple buffer
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );
	glutInitWindowPosition(this->position.xPosition,this->position.yPosition);
	glutInitWindowSize(this->size.width,this->size.height);
	glutCreateWindow(this->title);
}

void WindowGeneral::drawBackground()
{
	//Background color
	glClearColor(this->backgroundColor4Float.R,
				this->backgroundColor4Float.G,
				this->backgroundColor4Float.B,
				this->backgroundColor4Float.alpha);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//Get and Set
void WindowGeneral::setSize(int width, int height)
{
	this->size.width = width;
	this->size.height = height;
}

IntSize2D WindowGeneral::getSize()
{
	return this->size;
}

void WindowGeneral::setPosition(int xPosition, int yPosition)
{
	this->position.xPosition = xPosition;
	this->position.yPosition = yPosition;
}

IntPosition2D WindowGeneral::getPosition()
{
	return this->position;
}

void WindowGeneral::setTitle(char *title)
{
	this->title = title;
}

char* WindowGeneral::getTitle()
{
	return this->title;
}

void WindowGeneral::setBackgroundColor4Float(float R, float G, float B, float alpha)
{
	this->backgroundColor4Float.R = R;
	this->backgroundColor4Float.G = G;
	this->backgroundColor4Float.B = B;
	this->backgroundColor4Float.alpha = alpha;
}

FloatColor4D WindowGeneral::getBackgroundColor4Float()
{
	return this->backgroundColor4Float;
}
