#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>
#include <string>

//Importing our header files
#include "mouse.h"
//Importing our cpp files
#include "polygon.cpp"

using namespace std;


Mouse::Mouse()
{
	cout << "Mouse created!" << endl;
}

void Mouse::mouseClickHandler(int button, int state, int x, int y)
{
	//This is detected when we press a button
	if(state == GLUT_DOWN)
	{		
		//Left click
		if (button == GLUT_LEFT_BUTTON) 
		{
			leftClicked = true;	
		}
		//scroll click
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			scrollClicked = true;
		}
		//right click
		else 
		{
			rightClicked = true;
		}
	}
	else
	{		
		//Left click
		if (button == GLUT_LEFT_BUTTON) 
		{
			leftClicked = false;
		}
		//scroll click
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			scrollClicked = false;
		}
		//right click
		else 
		{
			rightClicked = false;
		}
	}
}







string keyName;
string command;
string action;

bool leftClicked;
bool rightClicked;
bool scrollClicked;

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
			leftClicked = true;	
		}
		//scroll click
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			keyName = "Scroll Key";
			scrollClicked = true;
		}
		//right click
		else 
		{
			keyName = "Right Key";
			rightClicked = true;
		}
	}
	else
	{
		command = "Released";
		
		//Left click
		if (button == GLUT_LEFT_BUTTON) 
		{
			keyName = "Left Key";
			leftClicked = false;
		}
		//scroll click
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			keyName = "Scroll Key";
			scrollClicked = false;
		}
		//right click
		else 
		{
			keyName = "Right Key";
			rightClicked = false;
		}
	}
	
	action += keyName;
	action += " ";
	action += command;
	
	//cout << action << endl;	
	
	mouseX = x;
	mouseY = y;
	//cout << mouseX << endl;	
	//cout << mouseY << endl;	
}
