#include "GL/freeglut.h"
#include "GL/gl.h"

#include "text.h"

Text::Text (string txt)
{
	this->text = txt;
}

void Text::setPosition(int posX, int posY)
{
	this->x = posX;
	this->y = posY;
}

void Text::setColor(float R, float G, float B)
{
	this->r = R;
	this->g = G;
	this->b = B;
}

void Text::draw()
{
	//Switch to window coordinates to render
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    glOrtho( 0, w, h, 0, -1, 1 );  //y axis goes from 0 to h from top to bottom
    
	glMatrixMode(GL_MODELVIEW);
	glColor3f(this->r, this->g, this->b);
	glPushMatrix();
    glLoadIdentity();
    
	glRasterPos2i(this->x, this->y);
	for(int i = 0; i < this->text.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, this->text[i]);
	}
	glPopMatrix();
}
