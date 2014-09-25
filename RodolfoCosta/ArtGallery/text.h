#pragma once

#include <string>
using namespace std;

class Text
{
private:
	int x, y;
	float r,g,b;
	string text;
public:
	Text(string txt);
	//Getters
	int getX() { return x; }
	int getY() { return y; }
	string getText() { return text; }
	//Setters
	void setPosition(int posX, int posY);
	void setColor(float R, float G, float B);
	
	void draw();
	
};
