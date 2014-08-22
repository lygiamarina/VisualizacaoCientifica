#include "WindowGeneral.h"
#include <GL/glut.h>

WindowGeneral::WindowGeneral()
{
	this->size = new int[2];
	size[0] = 600;
	size[1] = 480;

	this->position = new int[2];
	position[0] = 50;
	position[1] = 25;

	this->title = "Untitled";

	this->backgroundColor4Float = new float[4];
	backgroundColor4Float[0] = 0.95;
	backgroundColor4Float[1] = 0.95;
	backgroundColor4Float[2] = 0.95;
	backgroundColor4Float[3] = 0.95;
}

WindowGeneral::WindowGeneral(char *title)
{
	this->title = title;

	this->size = new int[2];
	size[0] = 600;
	size[1] = 480;

	this->position = new int[2];
	position[0] = 50;
	position[1] = 25;

	this->backgroundColor4Float = new float[4];
	backgroundColor4Float[0] = 0.95;
	backgroundColor4Float[1] = 0.95;
	backgroundColor4Float[2] = 0.95;
	backgroundColor4Float[3] = 0.95;
}

WindowGeneral::WindowGeneral(int *size, int *position, float* backgroundColor4Float)
{
	this->size = size;
	this->position = position;
	this->backgroundColor4Float = backgroundColor4Float;
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

char WindowGeneral::getTitle()
{
	return this->title;
}

void WindowGeneral::setBackgroundColor4Float(float *backgroundColor4Float)
{
	this->backgroundColor4Float = backgroundColor4Float;
}

float* getBackgroundColor4Float()
{
	return this->backgroundColor4Float;
}
