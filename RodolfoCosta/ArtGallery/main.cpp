#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>
#include <vector>

//Importing our cpp files
#include "mouse.cpp"

using namespace std;

void renderText(int x, int y, float r, float g, float b)
{
	//Switch to window coordinates to render
	glMatrixMode(GL_PROJECTION );
    glLoadIdentity();
    double w = glutGet( GLUT_WINDOW_WIDTH );
    double h = glutGet( GLUT_WINDOW_HEIGHT );
    glOrtho( 0, w, h, 0, -1, 1 );  //y axis goes from 0 to h from top to bottom
    
	glMatrixMode(GL_MODELVIEW);
	glColor3f(r, g, b);
	glPushMatrix();
    glLoadIdentity();
    
	glRasterPos2i(x, y);
	glColor3f(r, g, b);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)getAction); //bizarre string is drawn
	glPopMatrix();
	
	glutSwapBuffers();
}

void drawPolygon()
{
	for(int i = 0; i < polygon.size(); i++)
    {
		//cout << (*polygon[i]).getX() << endl;
		
		if(polygon.size() > 0)
		{
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glVertex2i((*polygon[i]).getX(), (*polygon[i]).getY());
			glEnd();
		}
	}
}

void renderFunction()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0, 1.0, 1.0);
    
	drawPolygon();
     
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(1, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glEnd();
    //glBegin(GL_POLYGON);
        //glVertex2f(100, 200);
        //glVertex2f(100, 400);
        //glVertex2f(200, 400);
        //glVertex2f(200, 200);
    //glEnd();
    
    glPopMatrix();
    
    renderText(10, 20, 0.0f, 0.0f, 255.0f);
    
    glutPostRedisplay();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("ArtGallery by Rodolfo");
    
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth (2.5);
    
    glutDisplayFunc(renderFunction);
    
    //Adding mouse events
    glutMouseFunc(mouseKeysHandler);
    
    //This is the last call to be made
    glutMainLoop();    
    return 0;
}
