#ifndef DRAWHANDLER_H
#define DRAWHANDLER_H

#include "PolygonVertex.h"
#include "WindowGeneral.h"
#include <vector>
#include <stdio.h>

class DrawHandler
{
private:
	static DrawHandler* currentInstance;

	WindowGeneral window;
	std::vector<PolygonVertex> &vertices;

	char* windowTitle;

public:
	DrawHandler(std::vector<PolygonVertex> &inVertices);

	void initialize();
	void drawOrder();
	static void drawCallback();
	void draw();

	//Set and Get
	void setWindow(WindowGeneral &window);
	WindowGeneral getWindow();
};

#endif