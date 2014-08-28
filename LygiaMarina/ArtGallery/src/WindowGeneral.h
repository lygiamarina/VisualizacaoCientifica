#ifndef WINDOWGENERAL_H
#define WINDOWGENERAL_H

#include "Structs.h"
#include <stdio.h>

class WindowGeneral
{
private:
	IntSize2D size;
	IntPosition2D position;
	char *title;
	FloatColor4D backgroundColor4Float;
	
public:
	WindowGeneral(char *title="Untitled");
	WindowGeneral(const WindowGeneral &window);

	//Screen
	void initialize();
	void drawBackground();

	//Get and Set
	void setSize(int width, int height);
	IntSize2D getSize();

	void setPosition(int xPosition, int yPosition);
	IntPosition2D getPosition();

	void setTitle(char *title);
	char* getTitle();

	void setBackgroundColor4Float(float R, float G, float B, float alpha);
	FloatColor4D getBackgroundColor4Float();
};

#endif