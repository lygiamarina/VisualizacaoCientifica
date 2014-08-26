#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>
#include <string>

//Importing our header files
#include "mouse.h"

using namespace std;

string keyName;
string command;
string action;

int mouseX;
int mouseY;

int getMouseX()
{
	return mouseX;
}

int getMouseY()
{
	return mouseY;
}

string getAction()
{
	return action;
}

void mouseKeysHandler(int button, int state, int x, int y) 
{
	keyName = "";
	command = "";
	action = "";
	
	//This is detected when we press a button
	if(state == GLUT_DOWN)
	{
		command = "Pressed";
		
		//Left click
		if (button == GLUT_LEFT_BUTTON) 
		{
			keyName = "Left Key";
		}
		//scroll click
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			keyName = "Scroll Key";
		}
		//right click
		else 
		{
			keyName = "Right Key";
		}
	}
	else
	{
		command = "Released";
		
		//Left click
		if (button == GLUT_LEFT_BUTTON) 
		{
			keyName = "Left Key";
		}
		//scroll click
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			keyName = "Scroll Key";
		}
		//right click
		else 
		{
			keyName = "Right Key";
		}
	}
	
	action += keyName;
	action += " ";
	action += command;
	
	cout << action << endl;	
	
	mouseX = x;
	mouseY = y;
	
	cout << mouseX << endl;	
	cout << mouseY << endl;	
}
