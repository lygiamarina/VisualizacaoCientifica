#include "Cell.h"
#include "convective.h"
#include "math.h"
#include <GL/glut.h>
#include <iostream>
#include <unistd.h>

#define WIDTH 640
#define HEIGHT 480
#define CELL_SIZE 20
#define INIT_VEL 16.0
#define GRAVITY 10.0

//--------------------------------------------------------------------//
// Global Variables 											      //
//--------------------------------------------------------------------//
int horizontalN;
int verticalN;
int t;
//Cell **currentGrid;
//Cell **previousGrid;
Cell currentGrid[26][34];
Cell previousGrid[26][34];

//--------------------------------------------------------------------//
// Drawing Function                                                   //
//--------------------------------------------------------------------//

void recalcVelocity()
{
	for (int i = 0; i < verticalN+2; i++)
	{
		for (int j = 0; j < horizontalN+2; j++)
		{
			previousGrid[i][j] = currentGrid[i][j];
		}
	}

	for (int i = 1; i < verticalN+1; i++)
	{
		for (int j = 1; j < horizontalN+1; j++)
		{
			bool borderI = false;
			bool borderJ = false;
			if (i == 1 || i == verticalN)
			{
				borderI = true;
			}
			if (j == 1 || j == horizontalN)
			{
				borderJ = true;
			}
			if (previousGrid[i][j].getIsFluidFlag())
			{
				//------------------------------------------------//
				// DUU Horizontal                                 //
				//------------------------------------------------//
				double LL = 0.0;
				double L = previousGrid[i][j-1].getEdgeVel("Left");
				double M = previousGrid[i][j].getEdgeVel("Left");
				double R = previousGrid[i][j+1].getEdgeVel("Left");
				double RR = 0.0;
				
				if (j > 1) 
				{
					LL = previousGrid[i][j-2].getEdgeVel("Left");
				}
				if (j < horizontalN)
				{
					RR = previousGrid[i][j+2].getEdgeVel("Left");
				}
				
				double duuHorizontal;
				duuHorizontal = DUU(LL, L, M, R, RR, 
						CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE, 
						1.0, borderJ);
						
				//------------------------------------------------//
				// DUV Horizontal                                 //
				//------------------------------------------------//
				double KLHor = previousGrid[i][j-1].getHorVelocity();
				double KLVer = previousGrid[i][j-1].getVerVelocity();
				double KL = sqrt(KLHor*KLHor + KLVer*KLVer);
				
				double KRHor = previousGrid[i][j].getHorVelocity();
				double KRVer = previousGrid[i][j].getVerVelocity();
				double KR = sqrt(KRHor*KRHor + KRVer*KRVer);
				
				double duvHorizontal;
				duvHorizontal = DUV(LL, L, M, R, RR, KL, KR, 
						CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE,
						1.0, borderJ);
						
				//------------------------------------------------//
				// DUU Vertical                                   //
				//------------------------------------------------//
				LL = 0.0;
				L = previousGrid[i-1][j].getEdgeVel("Down");
				M = previousGrid[i][j].getEdgeVel("Down");
				R = previousGrid[i+1][j].getEdgeVel("Down");
				RR = 0.0;
				
				if (i > 1) 
				{
					LL = previousGrid[i-2][j].getEdgeVel("Down");
				}
				if (i < verticalN)
				{
					RR = previousGrid[i+2][j].getEdgeVel("Down");
				}
				
				double duuVertical;
				duuVertical = DUU(LL, L, M, R, RR, 
						CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE, 
						1.0, borderI);
						
				//------------------------------------------------//
				// DUV Vertical                                   //
				//------------------------------------------------//
				KLHor = previousGrid[i-1][j].getHorVelocity();
				KLVer = previousGrid[i-1][j].getVerVelocity();
				KL = sqrt(KLHor*KLHor + KLVer*KLVer);
				
				KRHor = previousGrid[i][j].getHorVelocity();
				KRVer = previousGrid[i][j].getVerVelocity();
				KR = sqrt(KRHor*KRHor + KRVer*KRVer);
				
				double duvVertical;
				duvVertical = DUV(LL, L, M, R, RR, KL, KR, 
						CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE,
						1.0, borderI);
				
				double horizontalVel = KRHor - (duuHorizontal+duvVertical)*t;
				double verticalVel = KRVer - (duvHorizontal+duuVertical)*t;
				
				currentGrid[i][j].setEdgeVel("Left", horizontalVel);
				currentGrid[i][j].setEdgeVel("Down", verticalVel+GRAVITY*t);
				
				currentGrid[i][j-1].setEdgeVel("Right", horizontalVel);
				currentGrid[i-1][j].setEdgeVel("Up", verticalVel+GRAVITY*t);

			}
		}
	}
}

void drawVelocity()
{	
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	
	for(int i=1; i < verticalN+1; i++)
	{
		for (int j=1; j < horizontalN+1; j++)
		{
			if(currentGrid[i][j].getIsFluidFlag())
			{
				double horVelocity = currentGrid[i][j].getHorVelocity();
				double verVelocity = currentGrid[i][j].getVerVelocity();
				double *center = currentGrid[i][j].getCenter();
				
				glColor3f(0.0, 0.0, 1.0);
				glVertex2f(center[0], center[1]);
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(center[0]+horVelocity, center[1]+verVelocity);
			}
		}
	}
	
	glEnd ();
	glFlush();
	
	sleep(1);
	recalcVelocity();
	glutPostRedisplay();
	
}

int main(int argc, char **argv)
{	
	glutInit(&argc, argv);
	
	//----------------------------------------------------------------//
	// Initialization                                                 //
	//----------------------------------------------------------------//
	t = 1;
	
	horizontalN = WIDTH/CELL_SIZE;
	verticalN = HEIGHT/CELL_SIZE;

	/*currentGrid = new Cell*[verticalN+2];
	previousGrid = new Cell*[verticalN+2];*/
	
	for (int i = 1; i < verticalN+1; i++)
	{
		/*currentGrid[i] = new Cell[horizontalN+2];
		previousGrid[i] = new Cell[horizontalN+2];*/
		
		for (int j = 1; j < horizontalN+1; j++)
		{
			currentGrid[i][j].setCenter(CELL_SIZE*(j-1+0.5), CELL_SIZE*(i-1+0.5));
			if (i >= verticalN/4 && i <= 3*verticalN/4)
			{	
				currentGrid[i][0].setEdgeVel("Right", INIT_VEL);
				currentGrid[i][1].setEdgeVel("Left", INIT_VEL);
				currentGrid[i][1].setIsFluidFlag(true);
			}
		}
	}
	
	//----------------------------------------------------------------//
	// Window                                                         //
	//----------------------------------------------------------------//
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Visualizador de Velocidades - Fluid Simulation");
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)WIDTH, 0.0, (GLdouble)HEIGHT);
	
	//----------------------------------------------------------------//
	// Drawing                                                        //
	//----------------------------------------------------------------//
	glutDisplayFunc(drawVelocity);
	
	//----------------------------------------------------------------//
	// Velocity loop												  //
	//----------------------------------------------------------------//
	/*while(calcVel)
	{
		recalcVelocity();
		glutPostRedisplay();
		calcVel = false;
	}*/
	
	
	glutMainLoop();
	
	/*
	//----------------------------------------------------------------//
	// Deleting matrices                                              //
	//----------------------------------------------------------------//
	for (int i = 0; i < verticalN+2; i++)
	{
		delete [] currentGrid[i];
		delete [] previousGrid[i];
	}
	delete [] currentGrid;
	delete [] previousGrid;*/
	

	return 0;
}
