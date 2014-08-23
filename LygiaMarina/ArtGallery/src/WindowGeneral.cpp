#include "WindowGeneral.h"
#include <GL/glut.h>
#include <string>

WindowGeneral::WindowGeneral()
{
	this->size = new int[2];
	this->size[0] = 600;
	this->size[1] = 480;

	this->position = new int[2];
	this->position[0] = 50;
	this->position[1] = 25;

	this->title = "Untitled";

	this->backgroundColor4Float = new float[4];
	this->backgroundColor4Float[0] = 0.95;
	this->backgroundColor4Float[1] = 0.95;
	this->backgroundColor4Float[2] = 0.95;
	this->backgroundColor4Float[3] = 0.95;
}

WindowGeneral::WindowGeneral(char *title)
{
	this->title = title;

	this->size = new int[2];
	this->size[0] = 600;
	this->size[1] = 480;

	this->position = new int[2];
	this->position[0] = 50;
	this->position[1] = 25;

	this->backgroundColor4Float = new float[4];
	this->backgroundColor4Float[0] = 0.95;
	this->backgroundColor4Float[1] = 0.95;
	this->backgroundColor4Float[2] = 0.95;
	this->backgroundColor4Float[3] = 0.95;
}

WindowGeneral::WindowGeneral(int *size, int *position, float* backgroundColor4Float)
{
	this->size = size;
	this->position = position;
	this->backgroundColor4Float = backgroundColor4Float;
}

WindowGeneral::~WindowGeneral()
{
	WindowGeneral::deleteObj();
}

//Screen
void WindowGeneral::initialize()
{
	//Simple buffer
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowPosition(this->position[0],this->position[1]);
	glutInitWindowSize(this->size[0],this->size[1]);
	glutCreateWindow(this->title);
}

void WindowGeneral::drawBackground()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)this->size[0], 0.0, (GLdouble)this->size[1]);

	//Background color
	glClearColor(this->backgroundColor4Float[0],
				this->backgroundColor4Float[1],
				this->backgroundColor4Float[2],
				this->backgroundColor4Float[3]);

	glClear(GL_COLOR_BUFFER_BIT);
}

//Get and Set
void WindowGeneral::setSize(int *size)
{
	this->size = size;
}

int* WindowGeneral::getSize()
{
	return this->size;
}

void WindowGeneral::setPosition(int *position)
{
	this->position = position;
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

void WindowGeneral::setBackgroundColor4Float(float *backgroundColor4Float)
{
	this->backgroundColor4Float = backgroundColor4Float;
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
