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

	this->size = new int[2];
	*this->size = *window.size;
	*(this->size+1) = *(window.size+1);

	this->position = new int[2];
	*this->position = *window.position;
	*(this->position+1) = *(window.position+1);

	this->backgroundColor4Float = new float[4];
	for (int i = 0; i < 4; i++)
	{
		*(this->backgroundColor4Float+i) = *(window.backgroundColor4Float+i);
	}
}

WindowGeneral::~WindowGeneral()
{
	this->deleteObj();
}

//Screen
void WindowGeneral::initialize()
{
	//Simple buffer
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );
	glutInitWindowPosition(this->position[0],this->position[1]);
	glutInitWindowSize(this->size[0],this->size[1]);
	glutCreateWindow(this->title);
	gluOrtho2D(0.0, (GLdouble)this->size[0], 0.0, (GLdouble)this->size[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void WindowGeneral::drawBackground()
{
	//Background color
	glClearColor(this->backgroundColor4Float[0],
				this->backgroundColor4Float[1],
				this->backgroundColor4Float[2],
				this->backgroundColor4Float[3]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//Get and Set
void WindowGeneral::setSize(int width, int height)
{
	this->size = new int[2];
	this->size[0] = width;
	this->size[1] = height;
}

int* WindowGeneral::getSize()
{
	return this->size;
}

void WindowGeneral::setPosition(int xPosition, int yPosition)
{
	this->position = new int[2];
	this->position[0] = xPosition;
	this->position[1] = yPosition;
}

int* WindowGeneral::getPosition()
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
	this->backgroundColor4Float = new float[4];
	this->backgroundColor4Float[0] = R;
	this->backgroundColor4Float[1] = G;
	this->backgroundColor4Float[2] = B;
	this->backgroundColor4Float[3] = alpha;
}

float* WindowGeneral::getBackgroundColor4Float()
{
	return this->backgroundColor4Float;
}

//Common methods
void WindowGeneral::deleteObj()
{
	delete [] this->size;
	this->size = NULL;

	delete [] this->position;
	this->position = NULL;

	delete [] this->backgroundColor4Float;
	this->backgroundColor4Float = NULL;
}
