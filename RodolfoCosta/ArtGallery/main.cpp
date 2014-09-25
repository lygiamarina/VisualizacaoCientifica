#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>
#include <vector>

//Importing our cpp files
#include "mouse.cpp"

#include "text.cpp"

using namespace std;

Text *oie = new Text("oie");

void renderText(int x, int y, float r, float g, float b, string text)
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
	//glColor3f(r, g, b);
	for(int i = 0; i < text.length(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
	glPopMatrix();
	
	glutSwapBuffers();
}


// Returns 1 if the lines intersect, otherwise 0.
int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
    float p2_x, float p2_y, float p3_x, float p3_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        return 1; // Collision detected
    }

    return 0; // No collision
}



void drawPolygon()
{
	for(int i = 0; i < polygon.size(); i++)
    {
		glPointSize(5.0);
		glBegin(GL_POINTS);
		glVertex2i((*polygon[i]).getX(), (*polygon[i]).getY());
		glEnd();
		
		if(i > 0)
		{
			glBegin(GL_LINES);
			glVertex2f((*polygon[i-1]).getX(),(*polygon[i-1]).getY());
			glVertex2f((*polygon[i]).getX(), (*polygon[i]).getY());
			glEnd();
		}
		if(i > 2)
		{
			//If intersects
			if(get_line_intersection((*polygon[i-3]).getX(), (*polygon[i-3]).getY(), 
										  (*polygon[i-2]).getX(), (*polygon[i-2]).getY(), 
										  (*polygon[i-1]).getX(), (*polygon[i-1]).getY(), 
										  getMouseX(), getMouseY()))
			{
				//cout << "Intersected" << endl;
				//canAddVertex = false;
			}
			else
			{
				//canAddVertex = true;
			}
			//cout << get_line_intersection((*polygon[i-3]).getX(), (*polygon[i-3]).getY(), 
										  //(*polygon[i-2]).getX(), (*polygon[i-2]).getY(), 
										  //(*polygon[i-1]).getX(), (*polygon[i-1]).getY(), 
										  //(*polygon[i]).getX(), (*polygon[i]).getY()) << endl;
		}
	}
		
	if(leftClicked)
	{
		if(canAddVertex)
		{
			Vertex* newVertex = new Vertex(getMouseX(),getMouseY());  
			polygon.push_back(newVertex);
		}
		leftClicked = false;
	}
	//To close the polygon
	else if(rightClicked && polygon.size() > 2)
	{
		Vertex* newVertex = new Vertex((*polygon.front()).getX(), (*polygon.front()).getY());  
		polygon.push_back(newVertex);
		canAddVertex = false;
		rightClicked = false;
	}
	//To delete drawn polygon and start a new one
	else if(scrollClicked && polygon.size() > 0)
	{
		polygon.clear();
		canAddVertex = true;
		scrollClicked = false;
	}
	//cout << canAddVertex << endl;
}

void renderFunction()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(1.0, 1.0, 1.0);
    
	drawPolygon();
    
    //glBegin(GL_LINES);
    ////horizontal red line
    //glColor3f(1.0, 0, 0);
	//glVertex2f(100,300);
	//glVertex2f(200, 300);
	////vertical green line
	//glColor3f(0, 1.0, 0);
	//glVertex2f(150,200);
	//glVertex2f(150, 400);
	//glEnd();
	
	//glBegin(GL_LINES);
    ////horizontal blue line
    //glColor3f(0, 0, 1);
	//glVertex2f(100,440);
	//glVertex2f(200, 440);
	////horizontal white line
	//glColor3f(1, 1.0, 1);
	//glVertex2f(100,470);
	//glVertex2f(300,470);
	//glEnd();
    
    //oie->draw();
    
    renderText(10, 30, 0.0f, 0.0f, 255.0f, getAction());
    
    
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
    glLineWidth (20);
    
    oie->setPosition(100,300);
	oie->setColor(255.0f,0.0f,0.0f);
    
    glutDisplayFunc(renderFunction);
    
    //red line and green line test
    cout << get_line_intersection(100, 300, 200, 300, 150, 200, 150, 400) << endl;
    
    //blue line and white line test
    cout << get_line_intersection(100, 440, 200, 440, 100, 470, 300, 470) << endl;
    
    //Adding mouse events
    glutMouseFunc(mouseKeysHandler);
    
    //This is the last call to be made
    glutMainLoop();    
    return 0;
}
