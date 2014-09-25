#pragma once 

#include <string>
using namespace std;

class Mouse
{
private:
	int x, y;
	bool leftClicked, rightClicked, scrollClicked;
	bool mouseDragged, mouseStopped;
public:
	Mouse();
	
	//Getters
	int getX() { return x; }
	int getY() { return y; }
	bool getLeftClicked()   { return leftClicked;	}
	bool getRightClicked()  { return rightClicked;	}
	bool getScrollClicked() { return scrollClicked;	}
	bool getMouseDragged()  { return mouseDragged;  }
	bool getMouseStopped()  { return mouseStopped;	}
	
	//Setters
	int setX(int newX) { this->x = newX; }
	int setY(int newY) { this->y = newY; }
	bool setLeftClicked(bool newBool)   { this->leftClicked = newBool;	}
	bool setRightClicked(bool newBool)  { this->rightClicked = newBool;	}
	bool setScrollClicked(bool newBool) { this->scrollClicked = newBool;	}
	bool setMouseDragged(bool newBool)  { this->mouseDragged = newBool;  }
	bool setMouseStopped(bool newBool)  { this->mouseStopped = newBool;	}
	
	//Main methods
	void mouseClickHandler(int button, int state, int x, int y); 
};
int getMouseX();

int getMouseY();

string getAction();

void mouseKeysHandler(int button, int state, int x, int y);

