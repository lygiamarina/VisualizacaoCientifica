#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>

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
    glLoadIdentity();
    
	glRasterPos2i(x, y);
	glColor3f(r, g, b);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)getAction); //bizarre string is drawn
	
	glutSwapBuffers();
}

void renderFunction()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(100, 200);
        glVertex2f(100, 400);
        glVertex2f(200, 400);
        glVertex2f(200, 200);
    glEnd();
    
    glPopMatrix();
    
    renderText(10, 20, 255.0f, 0.0f, 0.0f);
    
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
    glutDisplayFunc(renderFunction);
    
    //Adding mouse events
    glutMouseFunc(mouseKeysHandler);
    
    //This is the last call to be made
    glutMainLoop();    
    return 0;
}
